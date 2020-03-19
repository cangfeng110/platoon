#pragma once

#include <string>
#include <memory>
#include <map>
#include <exception>
#include <iostream>
#include <thread>
#include <type_traits>
#include "Message_Channel.h" //contain all protobuf definition
#include "modules/ipc/include/lcm/lcm_channel.h"
#include "modules/ipc/include/TemplateBasedIPC/msgdispatcher.h"
#include "modules/ipc/include/TemplateBasedIPC/protowrapper.h"
#include "modules/ipc/include/TemplateBasedIPC/rwlock.h"
#include "modules/common/include/atd_app.h"
#include "modules/common/include/log.h"
#include "modules/ipc/include/channel_manager.h"



/**
 * @macro DecodeMap decode msg from map
 */
const bool DecodeMap = true;

/**
 * @macro Receive msg in Rte
 */
const int RecvInRte = 1;

namespace atd
{
namespace ipc
{

typedef std::shared_ptr<MsgDispatcherBase> pMsgDispatherBase;
typedef std::shared_ptr<std::thread> pThread;
typedef std::string ChannelName;
class SupperChannelManagerBase
{
public:
    SupperChannelManagerBase() {}
    virtual ~SupperChannelManagerBase();

};
/**
 * @brief The SupperChannelManager class  a Singleton class manager all subscribe and publish
 */
class SupperChannelManager: public SupperChannelManagerBase
{
public:
    ~SupperChannelManager() override;
    static std::shared_ptr<SupperChannelManager> getInstance();
    template <typename T>
    std::shared_ptr<MsgDispather_M<T>> getMsgDispather(const std::string ChannelName);
    template <typename T>
    pThread  enableThread(const std::string ChannelName);
    bool disableThread(const std::string ChannelName);
    std::map<std::string, pMsgDispatherBase>  dispacherMap;
private:
    SupperChannelManager();
    static std::shared_ptr<SupperChannelManager> instance;
    std::map<std::string, pThread> threadPool;
    RWLock m_RWLockthreadPool;
    RWLock m_RWLockDispatherMap;
};

template<typename T>
pThread SupperChannelManager::enableThread(const std::string ChannelName)
{
    if(ChannelName.empty())
    {
        return pThread(nullptr);
    }

    {
        auto guard = m_RWLockthreadPool.read_guard();
        auto iter = threadPool.find(ChannelName);
        if(iter != threadPool.end())
        {
            return iter->second;
        }
    }

    auto tempMsgDispacher = getMsgDispather<T>(ChannelName);
    pThread channelNameThread(new std::thread(&MsgDispather_M<T>::run, tempMsgDispacher));
    channelNameThread->detach();
    {
        auto guard = m_RWLockthreadPool.write_guard();
        threadPool.insert(std::make_pair(ChannelName, channelNameThread));
    }
    return channelNameThread;
}

template<typename T>
std::shared_ptr<MsgDispather_M<T>> SupperChannelManager::getMsgDispather(const std::string ChannelName)
{
    std::shared_ptr<MsgDispather_M<T>> p(nullptr);
    if(ChannelName.empty())
    {
        return p;
    }

    {
        auto guard = m_RWLockDispatherMap.read_guard();
        auto iter = dispacherMap.find(ChannelName);
        if(iter != dispacherMap.end())
        {
            return std::dynamic_pointer_cast<MsgDispather_M<T>>(iter->second);
        }
    }
    p.reset(new MsgDispather_M<T>(ChannelName));
    {
        auto guard = m_RWLockDispatherMap.write_guard();
        dispacherMap.insert(std::pair<std::string, pMsgDispatherBase>(ChannelName, p));
    }
    return p;
}


/**********************************user interface *************************************/

// Borrowed from C++ 14.
template <bool B, class T = void>
using enable_if_t = typename std::enable_if<B, T>::type;

/**
 * @caption Receive a message to a shared_ptr<Message>
 * @arg channelName :must clear konw the channel name for receiving messages ,suggestion :define channel name in "modules/ipc/include/lcm/lcm_channel.h"
 * @arg recMsg :the shard_ptr<Message> to hold the message
 * @arg OldMode :set default to false ,Due to the different encoding and decoding methods of the map module ,when receive proto msgs you should set it to true or use the macro DecodeMap
 * @arg secNum : set the lcm msg cache queue , When receiving a lcm message in the RTE ,you must set it to 1 or use macro RecInRte
 */
template <typename Message>
bool RxMsgPtr(ChannelName channelName, std::shared_ptr < enable_if_t < !std::is_base_of<google::protobuf::MessageLite,
              Message>::value,
              Message > > & recMsg, bool OldMode = false, int secNum = 30)
{
    std::shared_ptr<MsgDispather_M<Message>> ptrMsgDispather = SupperChannelManager::getInstance()->getMsgDispather<Message>(channelName);
    if(ptrMsgDispather == nullptr)
    {
        AERROR_LOG_DETAIL_EVERY(10) << " fail to read for get msgDispather for channel " << channelName << " error!";
        return false;
    }
    ptrMsgDispather->InitializeLCM(secNum);
    if(secNum > 1)
    {
        if(SupperChannelManager::getInstance()->enableThread<Message>(channelName) == nullptr)
        {
            AERROR_LOG_DETAIL << "fail to start thread to receive msg from " << channelName;
            return false;
        }
    }
    else
    {
        if(ptrMsgDispather->m_pLCM->handleTimeout(0) < 0)
        {
            AERROR_LOG_DETAIL_EVERY(10) << "An error accured while handle time out channel :" << channelName;
        }
    }
    ptrMsgDispather->m_lcmHandler.SetDecodeMode(OldMode);
    ptrMsgDispather->m_lcmHandler.UpdateDataStream();
    if(ptrMsgDispather->m_lcmHandler.DataValid())
    {
        recMsg = ptrMsgDispather->m_lcmHandler.Get();
        return true;
    }
    else
    {
//        recMsg = ptrMsgDispather->m_lcmHandler.Get();
        return false;
    }
}


template <typename Message>
bool RxMsgPtr(ChannelName channelName,
              std::shared_ptr<enable_if_t<std::is_base_of<google::protobuf::MessageLite, Message>::value, Message> >& recMsg, bool OldMode = false, int secNum = 30)
{
    std::shared_ptr<ProtoWrapper<Message> > temp(std::make_shared<ProtoWrapper<Message> >());
    if(RxMsgPtr<ProtoWrapper<Message> >(channelName, temp, OldMode, secNum))
    {
        recMsg = temp->getDataPtr();
        return true;
    }
    else
    {
//        recMsg = temp->getDataPtr();
        return false;
    }
}


/**
 * @caption Receive a message to a Message
 * @arg channelName :must clear konw the channel name for receiving messages ,suggestion :define channel name in "modules/ipc/include/lcm/lcm_channel.h"
 * @arg recMsg :the Message &(Message reference ) to hold the message
 * @arg OldMode :set default to false ,Due to the different encoding and decoding methods of the map module ,when receive proto msgs you should set it to true or use the macro DecodeMap
 * @arg secNum : set the lcm msg cache queue , When receiving a lcm message in the RTE ,you must set it to 1 or use macro RecInRte
 */
template <typename Message>
bool readMessageViaLcm(ChannelName channelName,
                       enable_if_t < !std::is_base_of<google::protobuf::MessageLite, Message>::value, Message > & recMsg, bool OldMode = false, int secNum = 30)
{
    std::shared_ptr<Message> temp(std::make_shared<Message>());
    if(RxMsgPtr<Message>(channelName, temp, OldMode, secNum))
    {
        recMsg = std::move(*temp);
        return true;
    }
    else
    {
        return false;
    }
}

template <typename Message>
bool readMessageViaLcm(ChannelName channelName,
                       enable_if_t<std::is_base_of<google::protobuf::MessageLite, Message>::value, Message>& recMsg, bool OldMode = false, int secNum = 30)
{
    ProtoWrapper<Message> temp;
    if(readMessageViaLcm<ProtoWrapper<Message> >(channelName, temp, OldMode, secNum))
    {
        recMsg = std::move(*(temp.getDataPtr()));
        return true;
    }
    else
    {
        return false;
    }
}


/**
 * @caption Receive a message to a shared_ptr<Message> Realize the @code ChannelManager::GET_XXXXX_Adapter()->GetData(); in the old ChannelManager interface
 * @arg channelName :must clear konw the channel name for receiving messages ,suggestion :define channel name in "modules/ipc/include/lcm/lcm_channel.h"
 * @arg recMsg :the shard_ptr<Message> to hold the message
 * @arg OldMode :set default to false ,Due to the different encoding and decoding methods of the map module ,when receive proto msgs you should set it to true or use the macro DecodeMap
 * @arg secNum : set the lcm msg cache queue , When receiving a lcm message in the RTE ,you must set it to 1 or use macro RecInRte
 */
template < typename Message >
bool getMsgPtr(ChannelName channelName, std::shared_ptr < enable_if_t < ! std::is_base_of<google::protobuf::MessageLite, Message>::value, Message > > &msgPtr )
{
    std::shared_ptr<MsgDispather_M<Message> > ptrMsgDispather = SupperChannelManager::getInstance()->getMsgDispather<Message>(channelName);
    if(ptrMsgDispather == nullptr)
    {
        AERROR_LOG_DETAIL << " fail to read for get msgDispather for channel " << channelName << " error!";
        return false;
    }
    if(ptrMsgDispather->m_lcmHandler.DataValid())
    {
        msgPtr = ptrMsgDispather->m_lcmHandler.Get();
        return true;
    }
    return false;
}

template < typename Message >
bool getMsgPtr(ChannelName channelName, std::shared_ptr<enable_if_t <std::is_base_of<google::protobuf::MessageLite, Message>::value,   Message> >& msgPtr )
{
    std::shared_ptr<ProtoWrapper< Message> > temp(std::make_shared<ProtoWrapper< Message> >());
    if(getMsgPtr<ProtoWrapper< Message> >(channelName, temp))
    {
        msgPtr = temp->getDataPtr();
        return true;
    }
    return false;
}


/**
 *  @note this is an upcoming feature to realize queue lcm msgs as you needed,if the demand is very clear.
 */
template <typename Message>
bool getDataStream(ChannelName channelName, DataStream<enable_if_t <! std::is_base_of<google::protobuf::MessageLite, Message>::value,   Message> >& out)
{
    std::shared_ptr<MsgDispather_M<Message>> ptrMsgDispather = SupperChannelManager::getInstance()->getMsgDispather<Message>(channelName);
    if(ptrMsgDispather == nullptr)
    {
        AERROR_LOG_DETAIL << " fail to read for get msgDispather for channel " << channelName << " error!";
        return false;
    }
    out = ptrMsgDispather->m_lcmHandler.GetDataStream();
    return true;
}

template <typename Message>
bool getDataStream(ChannelName channelName, DataStream<enable_if_t <std::is_base_of<google::protobuf::MessageLite, Message>::value,   Message> >& out)
{
    DataStream<ProtoWrapper<Message>> temp;
    if(getDataStream<ProtoWrapper< Message> >(channelName,temp))
    {
        out.setTimeStamp(std::move(temp.getTimeStamp()));
        out.setData(std::move(temp.getData()->getDataPtr()));
        return true;
    }
    return false;
}

/**
 *  @note this is an upcoming feature to realize queue lcm msgs as you needed,if the demand is very clear.
 */
template <typename Message>
bool setDataStream(ChannelName channelName, DataStream<enable_if_t <! std::is_base_of<google::protobuf::MessageLite, Message>::value,   Message> >& in)
{
    std::shared_ptr<MsgDispather_M<Message>> ptrMsgDispather = SupperChannelManager::getInstance()->getMsgDispather<Message>(channelName);
    if(ptrMsgDispather == nullptr)
    {
        AERROR_LOG_DETAIL << " fail to read for get msgDispather for channel " << channelName << " error!";
        return false;
    }
    if(ptrMsgDispather->m_lcmHandler.setDataStream(in))
    {
        return true;
    }
    return false;
}

template <typename Message>
bool setDataStream(ChannelName channelName, DataStream<enable_if_t <std::is_base_of<google::protobuf::MessageLite, Message>::value,   Message> >& in)
{
    DataStream<ProtoWrapper<Message> > tempDataStream;

    std::shared_ptr<ProtoWrapper<Message> > tempWrapper(std::make_shared<ProtoWrapper<Message> >());
    auto ptr=in.getData();
    tempWrapper->setData(ptr);


    tempDataStream.setData(tempWrapper);
    tempDataStream.setTimeStamp(std::move(in.getTimeStamp()));
    if(setDataStream<ProtoWrapper< Message> >(channelName,tempDataStream))
    {
        return true;
    }
    return false;
}

/**
 * @caption Send a message to specilized channel
 * @arg channelName :must clear konw the channel name for sending messages ,suggestion :define channel name in "modules/ipc/include/lcm/lcm_channel.h"
 * @arg sendMsg :the Message& to hold the message
 * @note  Once you use this interface to send proto messages,then you shouldn't use @macro DecodeMap when receive messages
 */
template <typename Message>
bool sendMessageViaLcm(ChannelName channelName,  enable_if_t < !std::is_base_of<google::protobuf::MessageLite,
                       Message>::value,
                       const Message > & sendMsg)
{
    std::shared_ptr<MsgDispather_M<Message>> ptrMsgDispather = SupperChannelManager::getInstance()->getMsgDispather<Message>(channelName);
    if(nullptr == ptrMsgDispather)
    {
        AERROR_LOG_DETAIL << " fail to send for get msgDispather for channel " << channelName << " error!";
        return false;
    }
    ptrMsgDispather->send(sendMsg);
    return true;
}

template <typename Message>
bool sendMessageViaLcm(ChannelName channelName,  enable_if_t<std::is_base_of<google::protobuf::MessageLite,
                       Message>::value,
                       Message>& sendMsg)
{
    ProtoWrapper<Message> tempWrapper;
    std::shared_ptr<Message> ptr = std::make_shared<Message>(sendMsg);
    tempWrapper.setData(ptr);
    return sendMessageViaLcm<ProtoWrapper<Message> >(channelName, tempWrapper);
}

/**
 *  @note this is an upcoming feature to realize personalize the read and send in your control
 */
template <typename Message>
std::shared_ptr<MsgDispather_M<Message>> getMsgDispather(ChannelName channelName)
{
    std::shared_ptr<MsgDispather_M<Message>> ptrMsgDispather = SupperChannelManager::getInstance()->getMsgDispather<Message>(channelName);
    if(ptrMsgDispather == nullptr)
    {
        AERROR_LOG_DETAIL << " fail to read for get msgDispather for channel " << channelName << " error!";
    }
    return ptrMsgDispather;
}

/**
 * @caption encode the proto or lcm msg to bytes array
 * @arg msgToEncode
 * @arg buf a pointer to bytes array ,@note must ensure the buf has Arrange enough space in advance
 */
template<typename Message>
bool dumpToArray(enable_if_t<std::is_base_of<google::protobuf::MessageLite,
                 Message>::value,
                 Message>& msgToEncode, void *buf)
{
    ProtoWrapper<Message> tempWrapper{};
    std::shared_ptr<Message> tempPtr = std::make_shared<Message>(std::move(msgToEncode));
    tempWrapper.setData(tempPtr);
    return tempWrapper.dumpToArray(buf);
}

template<typename Message>
bool dumpToArray(enable_if_t < !std::is_base_of<google::protobuf::MessageLite,
                 Message>::value,
                 Message > & msgToEncode, void *buf)
{
    return DecodeAndEncodeFromArray::dumpToArray<Message>(msgToEncode, buf);
}

/**
 * @caption decode the proto or lcm msg from bytes array
 * @arg msgDecodeTo
 * @arg buf a pointer to bytes array
 * @arg offset defines where to start decode if the buf is encoded with @code dumpToArray, then you don't need to set offset and maxLen
 */
template<typename Message>
bool decodeFromArray(enable_if_t<std::is_base_of<google::protobuf::MessageLite,
                     Message>::value,
                     Message>& msgDecodeTo, void *buf, int offset, int maxLen)
{
    ProtoWrapper<Message> tempWrapper;
    if(tempWrapper.decode(buf, offset, maxLen) != -1)
    {
        msgDecodeTo = tempWrapper.getData();
        return true;
    }
    AERROR_LOG_DETAIL_EVERY(10) << "Error_decoding " << tempWrapper.getTypeName() << " from array data!" << std::endl;
    return false;
}

template<typename Message>
bool decodeFromArray(const enable_if_t < !std::is_base_of<google::protobuf::MessageLite,
                     Message>::value,
                     Message > & msgDecodeto, void *buf, int offset, int maxLen)
{
    if(msgDecodeto.decode(buf, offset, maxLen) != -1)
    {
        return true;
    }
    AERROR_LOG_DETAIL_EVERY(10) << "Error_decoding " << msgDecodeto.getTypeName() << " from array data!" << std::endl;
    return false;
}

template<typename Message>
bool decodeFromArray(enable_if_t<std::is_base_of<google::protobuf::MessageLite,
                     Message>::value,
                     Message>& msgDecodeTo, void *buf)
{
    ProtoWrapper<Message> tempWrapper;

    if(DecodeAndEncodeFromArray::decodeFromArray<ProtoWrapper<Message> >(tempWrapper, buf))
    {
        msgDecodeTo = tempWrapper.getData();
        return true;
    }

    AERROR_LOG_DETAIL_EVERY(10) << "Error_decoding " << tempWrapper.getTypeName() << " from array data!" << std::endl;
}


template<typename Message>
bool decodeFromArray(enable_if_t < !std::is_base_of<google::protobuf::MessageLite,
                     Message>::value,
                     Message > & msgDecodeto, void *buf)
{
    if(DecodeAndEncodeFromArray::decodeFromArray(msgDecodeto, buf))
    {
        return true;
    }
    AERROR_LOG_DETAIL_EVERY(10) << "Error_decoding " << msgDecodeto.getTypeName() << " from array data!" << std::endl;
    return false;
}

template<typename Message>
bool fillHeader(enable_if_t<std::is_base_of<google::protobuf::MessageLite,
                     Message>::value,
                     Message>& msgToFillHeader,ChannelName channelName)
{
    std::shared_ptr<MsgDispather_M<ProtoWrapper<Message> > > ptrMsgDispather = SupperChannelManager::getInstance()->getMsgDispather<ProtoWrapper<Message> >(channelName);
    if(ptrMsgDispather == nullptr)
    {
        AERROR_LOG_DETAIL_EVERY(10) << " fail to read for get msgDispather for channel " << channelName << " error!";
        return false;
    }
    auto seqNum= ptrMsgDispather->getSequenceNum();
    DecodeAndEncodeFromArray::fillHeader(msgToFillHeader,seqNum);
    ptrMsgDispather->setSequencNum(seqNum);
    return true;
}

template<typename Message>
bool fillHeader(enable_if_t<std::is_base_of<google::protobuf::MessageLite,
                     Message>::value,
                     Message>& msgToFillHeader,ChannelName channelName,const uint64_t& timeStamp)
{
    std::shared_ptr<MsgDispather_M<ProtoWrapper<Message> > > ptrMsgDispather = SupperChannelManager::getInstance()->getMsgDispather<ProtoWrapper<Message> >(channelName);
    if(ptrMsgDispather == nullptr)
    {
        AERROR_LOG_DETAIL_EVERY(10) << " fail to read for get msgDispather for channel " << channelName << " error!";
        return false;
    }
    auto seqNum= ptrMsgDispather->getSequenceNum();
    DecodeAndEncodeFromArray::fillHeader(msgToFillHeader,timeStamp,seqNum);
    ptrMsgDispather->setSequencNum(seqNum);
    return true;
}

template<typename Message>
bool fillHeader(enable_if_t<! std::is_base_of<google::protobuf::MessageLite,
                     Message>::value,
                     Message>& msgToFillHeader,ChannelName channelName)
{
    return false;
}

template<typename Message>
bool fillHeader(enable_if_t< ! std::is_base_of<google::protobuf::MessageLite,
                     Message>::value,
                     Message>& msgToFillHeader,ChannelName channelName,const uint64_t& timeStamp)
{
    return false;
}

}
}


