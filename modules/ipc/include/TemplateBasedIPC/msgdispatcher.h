#ifndef MSGDISPATCHER_H
#define MSGDISPATCHER_H

#include <mutex>
#include <memory>
// lcm related
#include <lcm/lcm-cpp.hpp>
#include "modules/common/include/registerer.h"
#include "modules/common/include/log.h"
#include "modules/common/include/easylogging++.h"
#include "modules/ipc/include/TemplateBasedIPC/datastreamcontainer.h"
#include "modules/ipc/include/protobuf/msg_array.hpp"
#include "modules/ipc/include/TemplateBasedIPC/rwlock.h"
#include "modules/common/include/util/util.h"
#include <iostream>

namespace  atd
{
namespace ipc
{
/**
 * @brief The MsgDispatcherBase class
 */

using namespace atd::common::util;

class MsgDispatcherBase
{
public:
    MsgDispatcherBase(std::string channelname);
    virtual ~MsgDispatcherBase() = default;
    virtual void run() = 0;
    std::shared_ptr<lcm::LCM> m_pLCM;
    std::string mychannelname;
    RWLock m_rwLock_Initialized;
    bool m_bInitialized;
};


template<typename T>
class MsgHandler_M
{
public:
    MsgHandler_M();
    ~MsgHandler_M() {}
    void MsgCallback(const lcm::ReceiveBuffer *recvBuf, const std::string &channelName);
    DataStreamContainer<DataStream<T> > m_DataStreamContainer;
    void Get(T & result);

    std::shared_ptr<T> Get()
    {
        auto guard = m_RWLock_data.read_guard();
        return std::move(m_DataStreamTransfer.getData());
    }

    DataStream<T> GetDataStream();
    bool setDataStream(DataStream<T>& dataStream);
    bool DataValid();
    void SetDecodeMode(bool Mode);
    void SetOutPutDebug(bool Mode);
    bool UpdateDataStream();

private:
    bool OldMode;
    bool OutPutDebug;
    std::unique_ptr<T> m_buffer;
    custom::example::msg_array m_buffer_array;
    RWLock m_RWLock_data;
    RWLock m_RWLock_dataTransfer;
    RWLock m_RWLock_DecodeMode;
    std::mutex m_mutex;
    std::mutex m_mutex_DecodeMode;
    std::mutex m_mutex_OutPutMode;
    DataStream<T> m_DataStreamTransfer;
    DataStream<T> m_DataStrem;
    int64_t recvTime;
public:
};


template<typename T>
MsgHandler_M<T>::MsgHandler_M(): OutPutDebug(false), OldMode(false), m_buffer(make_unique<T>(T{})), m_DataStrem(), m_DataStreamTransfer()
{

    m_buffer_array = {};
}

template<typename T>
void MsgHandler_M<T>::MsgCallback(const lcm::ReceiveBuffer *recvBuf, const std::string &channelName)
{
    bool isOldMode;
    {
        auto guard = m_RWLock_DecodeMode.read_guard();
        isOldMode = OldMode;
    }
    if(isOldMode)
    {
        auto guard = m_RWLock_data.write_guard();
        if(-1 == m_buffer_array.decode(recvBuf->data, 0, recvBuf->data_size))
        {
            AERROR << "Channel " << channelName << " couldn't be decode as msg_array" << std::endl;
//            m_DataStream.setDataValid(false);
            return;
        }
        if(m_buffer_array.proto_encoded_arr.size() > 0)
        {
            if(-1 == m_buffer->decode(&(m_buffer_array.proto_encoded_arr[0]), 0, m_buffer_array.arr_size))
            {
                AERROR << "Something erro in parse msg frome " << channelName << std::endl;
//                m_DataStream.setDataValid(false);
                return;
            }
        }
        else
        {
//            m_DataStream.setDataValid(false);
            return;
        }
        recvTime = recvBuf->recv_utime;
        return;
    }

    {
        auto guard = m_RWLock_data.write_guard();
        if(-1 == m_buffer->decode(recvBuf->data, 0, recvBuf->data_size))
        {
//            LOG_IF(OutPutDebug, INFO) << "Something erro in parse msg from " << channelName << std::endl;
            AERROR<<"Error decoding "<< channelName<<"  AS: "<<m_buffer->getTypeName();
//            m_DataStream.setDataValid(false);
            return;
        }
        recvTime = recvBuf->recv_utime;
        return;
    }
}

template<typename T>
void MsgHandler_M<T>::Get(T &result)
{
    UpdateDataStream();
    {
        auto guard = m_RWLock_data.read_guard();
        if(m_DataStreamTransfer.IsValid() && m_DataStreamTransfer.data)
        {
            result = std::move(*m_DataStreamTransfer.data);
        }
    }
}



template<typename T>
DataStream<T> MsgHandler_M<T>::GetDataStream()
{
    auto guard = m_RWLock_data.read_guard();
    return std::move(m_DataStreamTransfer);
}

template<typename T>
bool MsgHandler_M<T>::setDataStream(DataStream<T>& dataStream)
{
    {
        auto guard = m_RWLock_data.write_guard();
        // Using receive time stamp to judge whether data valid

        if(m_DataStreamTransfer.isFresher(dataStream))
        {
            m_DataStreamTransfer.setTimeStamp(dataStream.getTimeStamp());
            m_DataStreamTransfer.setDataValid(true);
            m_DataStreamTransfer.setData(std::move(dataStream.getData()));
            m_buffer = make_unique<T>();
            return true;
        }
        else
        {
            m_DataStreamTransfer.setDataValid(false);
            return false;
        }
    }
}

template<typename T>
bool MsgHandler_M<T>::DataValid()
{
    auto guard = m_RWLock_data.read_guard();
    return m_DataStreamTransfer.IsValid();
}

template<typename T>
void MsgHandler_M<T>::SetDecodeMode(bool Mode)
{
    {
        auto guard = m_RWLock_DecodeMode.read_guard();
        if(OldMode & Mode)
        {
            return;
        }
    }
    {
        auto guard = m_RWLock_DecodeMode.write_guard();
        OldMode = Mode;
    }
}

template<typename T>
void MsgHandler_M<T>::SetOutPutDebug(bool Mode)
{
    if(OutPutDebug & Mode)
    {
        return;
    }
    {
        std::lock_guard<std::mutex> lock(m_mutex_OutPutMode);
        OutPutDebug = Mode;
    }
}

template<typename T>
bool MsgHandler_M<T>::UpdateDataStream()
{
    {
        auto guard = m_RWLock_data.write_guard();
        // Using receive time stamp to judge whether data valid
        if(m_DataStreamTransfer.setTimeStamp(recvTime))
        {
            m_DataStreamTransfer.setDataValid(true);
            m_DataStreamTransfer.setData(std::move(m_buffer));
            m_buffer = make_unique<T>();
            return true;
        }
        else
        {
            m_DataStreamTransfer.setDataValid(false);
            return false;
        }
    }
}

typedef uint8_t (*channelReciever)(void*);
typedef uint8_t (*channelSender)(void*);

template<typename T>
class MsgDispather_M : public MsgDispatcherBase
{
public:
    MsgDispather_M(const std::string channelName): MsgDispatcherBase(channelName), m_lcmHandler{}
    {}
    void send(const T& sendMsg);
    void run() override;
    void InitializeLCM(int seqNum = 30);
    MsgHandler_M<T> m_lcmHandler;
    int getSequenceNum();
    void setSequencNum(const int64_t seqNum);
private:
    int m_intSequenceNum;
    channelReciever m_pReciveFunc;
    channelSender m_pSenderFunc;
    RWLock m_RWLock_SequenceNum;

    /**
     * @brief subMap used to unsubscriber to some specfied channel
     */
//    std::map<const std::string,std::shared_ptr<lcm::Subscription>> subMap;
//    std::shared_ptr<lcm::Subscription> pSub;
};

template<typename T>
void MsgDispather_M<T>::send(const T &sendMsg)
{
    int datalen = sendMsg.getEncodedSize();
    std::unique_ptr<uint8_t []> buf_(new uint8_t[datalen]);
    sendMsg.encode(buf_.get(), 0, datalen);
    m_pLCM->publish(mychannelname, buf_.get(), datalen);
}

template<typename T>
void MsgDispather_M<T>::run()
{
//    InitializeLCM();
    while(0 == m_pLCM->handle())
    {


    }
}

template<typename T>
void MsgDispather_M<T>::InitializeLCM(int seqNum)
{
    {
        auto guard = m_rwLock_Initialized.read_guard();
        if(m_bInitialized)
        {
            return;
        }
    }
    {
        auto guard = m_rwLock_Initialized.write_guard();
        if(!m_pLCM->good())
        {
            LOG(ERROR) << "initialize communication error, please check the ethernet connection";
            //        AERROR << "initialize communication error, please check the ethernet connection";
            //PrintErrorInfo("initialize communication error, please check the ethernet connection", __FILE__, __FUNCTION__, __LINE__);
            m_bInitialized = false;
        }
        auto sub = m_pLCM->subscribe(mychannelname, &MsgHandler_M<T>::MsgCallback, &m_lcmHandler);
        //    pSub.reset();
        sub->setQueueCapacity(seqNum);//seqNum=1;
        {
            m_bInitialized = true;
        }
    }
}

template<typename T>
int MsgDispather_M<T>::getSequenceNum()
{
    auto guard=m_RWLock_SequenceNum.read_guard();
    return m_intSequenceNum;
}

template<typename T>
void MsgDispather_M<T>::setSequencNum(const int64_t seqNum)
{
    auto guard=m_RWLock_SequenceNum.write_guard();
    m_intSequenceNum=static_cast<int>(seqNum);
}
}
}


#endif // MSGDISPATCHER_H
