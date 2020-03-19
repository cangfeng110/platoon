#ifndef MODULES_IPC_PROTO_CPROTOSUBSCRIBER_H
#define MODULES_IPC_PROTO_CPROTOSUBSCRIBER_H

#include <lcm/lcm-cpp.hpp>
#include <mutex>
#include "modules/ipc/include/protobuf/msg_array.hpp"
#include "modules/common/include/status/status.h"
#include <google/protobuf/message_lite.h>


/**
 * @namespace atd::ipc
 * @brief atd::ipc
 */
namespace atd {
namespace ipc {

using atd::common::Status;

//h-fatherclass
class CPROTOSubscriber
{
public:
    CPROTOSubscriber(const std::string channelname);
    CPROTOSubscriber(bool isWatchdog, int valid,const std::string channelname,void *(*callMainHander)(const std::string channelname));
    ~CPROTOSubscriber();
    virtual void InitializeLCM()=0;

    void run();
    bool IsInitialized();
    void *(*callMainHander)(const std::string channelname);
    std::string mychannelname;
    int m_valid;
    int m_realcount;
    bool IsValid;
    bool isenable;
    bool validmessage();

    lcm::LCM* m_pLCM;
    bool m_bInitialized;
    bool m_isWatchdog;

private:

};

//cpp-father
inline CPROTOSubscriber::CPROTOSubscriber(const std::string channelname)
{
    m_pLCM = new lcm::LCM("udpm://239.255.76.67:7667?ttl=3");
    mychannelname="PROTO_"+channelname;
    IsValid=false;
    m_bInitialized = false;
    isenable=true;
}

inline CPROTOSubscriber::CPROTOSubscriber(bool _isWatchdog, int _valid, const std::string channelname,void* (*_callMainHander)(const std::string))
{
    m_pLCM = new lcm::LCM("udpm://239.255.76.67:7667?ttl=3");
    mychannelname="PROTO_"+channelname;
    IsValid=false;
    m_bInitialized = false;
    m_valid=_valid;
    m_realcount=_valid;
    m_isWatchdog=_isWatchdog;
    isenable=true;
    callMainHander=_callMainHander;
}

inline CPROTOSubscriber::~CPROTOSubscriber()
{

}


inline void CPROTOSubscriber::run()
{
    InitializeLCM();
    while(0==m_pLCM->handle())
    {
        if(callMainHander!=NULL)
        {
            callMainHander(mychannelname);
        }
        m_realcount=m_valid;
        IsValid=true;
    }
}


inline bool CPROTOSubscriber::IsInitialized()
{
    return m_bInitialized;
}

inline bool CPROTOSubscriber::validmessage()
{
    if(m_isWatchdog)
    {
        m_realcount--;
        if(m_realcount<0)
        {
            m_realcount=-1;
            //m_lcmHandler.m_result=NULL;
            IsValid=false;
            return false;
        }
        return true;
    }
    else
    {
        return true;
    }
}


//h-T
template<class T>
class CPROTOHandler_M
{
public:
    CPROTOHandler_M();
    void MsgCallback(const lcm::ReceiveBuffer* recvBuf, const std::string& channelName, const custom::example::msg_array* msg);
    void Get(T &m_result);
    void Get_Array(uint8_t inputmsg[]);
    //T m_result;
private:
    T m_buffer;
    custom::example::msg_array m_buffer_array;
    std::mutex m_mutex;
};

template<class T>
class CPROTOSubscriber_M:public CPROTOSubscriber
{
public:
    CPROTOSubscriber_M(const std::string channelname);
    CPROTOSubscriber_M(bool isWatchdog, int valid,const std::string channelname,void *(*callMainHander)(const std::string channelname));
    ~CPROTOSubscriber_M();
    void Send(T &sendmsg);
    void Send_Array(const uint8_t inputmsg[],const int msgsize);
    virtual void InitializeLCM();
    CPROTOHandler_M<T> m_lcmHandler;

    Status getProtoMsg(const uint8_t inputmsg[],const int fix_size,T* m_buffer);
    int findsizegap(const T* send_msg,const int fix_size);
    int sendProtoMsg(const T* send_msg,const int fix_size,uint8_t inputmsg[]);
    int getMsgSize(const T* send_msg);

    /**
     * @brief InitializeLCM_with_low  In order to receive lcm messages in RTE envirenment ,need to set the setQueueCapacity to 1;
     */
    void InitializeLCM_with_setQueueCapacity();


private:

};


//cpp-CPROTOHandler_M
template<class T>
CPROTOHandler_M<T>::CPROTOHandler_M()
{

}

template<class T>
void CPROTOHandler_M <T>::MsgCallback(const lcm::ReceiveBuffer *recvBuf, const std::string &channelName, const custom::example::msg_array *msg)
{
    m_mutex.lock();

    ////get the protomsg
    //    std::string proto_encoded_msg(msg->proto_encoded_arr.begin(), msg->proto_encoded_arr.end());
    //    if(!((google::protobuf::Message*)&m_buffer)->ParseFromString(proto_encoded_msg))
    //    {
    //        cout<<"error:proto trans"<<endl;
    //    }
    m_buffer_array=*msg;
    m_mutex.unlock();
}

template<class T>
void CPROTOHandler_M <T>::Get(T &m_result)
{
    m_mutex.lock();
    m_result=m_buffer;
    m_mutex.unlock();
}


template<class T>
void CPROTOHandler_M <T>::Get_Array(uint8_t inputmsg[])
{
    m_mutex.lock();
    for(int i=0;i<m_buffer_array.proto_encoded_arr.size();i++)
    {
        inputmsg[i]=m_buffer_array.proto_encoded_arr[i];
    }
    m_mutex.unlock();
}

//cpp-CPROTOSubscriber_M
template<class T>
CPROTOSubscriber_M <T>::CPROTOSubscriber_M(const std::string channelname):CPROTOSubscriber(channelname)
{

}

template<class T>
CPROTOSubscriber_M <T>::CPROTOSubscriber_M(bool isWatchdog, int valid, const std::string channelname, void *(*callMainHander)(const std::string))
    :CPROTOSubscriber(isWatchdog,valid,channelname,callMainHander)
{

}

template<class T>
CPROTOSubscriber_M <T>::~CPROTOSubscriber_M()
{

}

template<class T>
void CPROTOSubscriber_M <T>::Send(T &send_msg)
{
    std::string encoded_msg_by_protobuf;
    ((google::protobuf::MessageLite*)&send_msg)->SerializeToString(&encoded_msg_by_protobuf);

    custom::example::msg_array lcm_msg_arr;
    lcm_msg_arr.proto_index = 0;
    lcm_msg_arr.arr_size = static_cast<int>(((google::protobuf::MessageLite*)&send_msg)->ByteSizeLong());
    lcm_msg_arr.proto_encoded_arr.reserve(((google::protobuf::MessageLite*)&send_msg)->ByteSizeLong());
    lcm_msg_arr.proto_encoded_arr.assign(encoded_msg_by_protobuf.begin(), encoded_msg_by_protobuf.end());

    m_pLCM->publish(mychannelname,&lcm_msg_arr);
}

template<class T>
void CPROTOSubscriber_M <T>::Send_Array(const uint8_t inputmsg[],const int msgsize)
{
    custom::example::msg_array lcm_msg_arr;
    lcm_msg_arr.proto_index = 0;
    lcm_msg_arr.arr_size = msgsize;
    lcm_msg_arr.proto_encoded_arr.reserve(msgsize);
    lcm_msg_arr.proto_encoded_arr.assign(inputmsg,inputmsg+msgsize);

    m_pLCM->publish(mychannelname,&lcm_msg_arr);
}


template<class T>
void CPROTOSubscriber_M <T>::InitializeLCM()
{
    if(!m_pLCM->good())
    {
        //PrintErrorInfo("initialize communication error, please check the ethernet connection", __FILE__, __FUNCTION__, __LINE__);
        m_bInitialized = false;
    }

    m_pLCM->subscribe(mychannelname, &CPROTOHandler_M<T>::MsgCallback, &m_lcmHandler);
    m_bInitialized = true;
}


template<class T>
Status CPROTOSubscriber_M <T>::getProtoMsg(const uint8_t inputmsg[],const int fix_size,T* m_buffer)
{
    if(!((google::protobuf::MessageLite*)m_buffer)->ParseFromArray(inputmsg,fix_size))
    {
        return Status(atd::common::ErrorCode::HTTP_LOGIC_ERROR, "the msg channel:"+mychannelname+" getProtoMsg() trans fail");
    }
    else
    {
        return Status(atd::common::ErrorCode::OK,"none");
    }
}

template<class T>
int CPROTOSubscriber_M <T>::findsizegap(const T* send_msg,const int fix_size)
{
    int cur_size = static_cast<int>(((google::protobuf::MessageLite*)send_msg)->ByteSizeLong());
    int reserved_min_buffer=6;
    if(cur_size+reserved_min_buffer+4>fix_size)
    {
        //<<"the channel:"<<mychannelname<<" msg size is out of range, cur_size(+12):"<<cur_size<<"byte,fix_size:"<<fix_size<<"byte";
        return -1;
    }
    else if(cur_size>1024*30)  //>20MB
    {
        //<<"the channel:"<<mychannelname<<" msg size is HUGE, cur_size:"<<cur_size<<"byte,fix_size:"<<fix_size<<"byte";
        return -2;
    }
    else
    {
        int gap=fix_size-cur_size-reserved_min_buffer;
        if(gap>1024)
        {
            //<<"the channel:"<<mychannelname<<" msg fix_size is not reasonable, cur_size(+4):"<<cur_size<<"byte,fix_size:"<<fix_size<<"byte";
        }
        return gap-4;//for buffer of reserved_bit
    }
}

template<class T>
int CPROTOSubscriber_M <T>::sendProtoMsg(const T* send_msg,const int fix_size,uint8_t inputmsg[])
{
    //std::string encoded_msg_by_protobuf;
    ((google::protobuf::MessageLite*)send_msg)->SerializeToArray(inputmsg,fix_size);
    //inputmsg=encoded_msg_by_protobuf;
    //strcpy(inputmsg, encoded_msg_by_protobuf.c_str());
}

template<class T>
int CPROTOSubscriber_M <T>::getMsgSize(const T* send_msg)
{
    return static_cast<int>(((google::protobuf::MessageLite*)send_msg)->ByteSizeLong());
}

template<class T>
void CPROTOSubscriber_M<T>::InitializeLCM_with_setQueueCapacity()
{

    if(!m_pLCM->good())
    {
        //PrintErrorInfo("initialize communication error, please check the ethernet connection", __FILE__, __FUNCTION__, __LINE__);
        m_bInitialized = false;
    }

    lcm::Subscription* p=m_pLCM->subscribe(mychannelname, &CPROTOHandler_M<T>::MsgCallback, &m_lcmHandler);
    p->setQueueCapacity(1);
    m_bInitialized = true;
}




}  // namespace ipc
}  // namespace atd

#endif // MODULES_IPC_PROTO_CPROTOSUBSCRIBER_H
