#ifndef MODULES_IPC_LCM_CLCMSUBSCRIBER_H
#define MODULES_IPC_LCM_CLCMSUBSCRIBER_H

#include <lcm/lcm-cpp.hpp>
#include <mutex>

/**
 * @namespace atd::ipc
 * @brief atd::ipc
 */
namespace atd {
namespace ipc {

//h-fatherclass
class CLCMSubscriber
{
public:
    CLCMSubscriber(const std::string channelname);
    CLCMSubscriber(bool isWatchdog, int valid,const std::string channelname,void *(*callMainHander)(const std::string channelname));
    ~CLCMSubscriber();
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
inline CLCMSubscriber::CLCMSubscriber(const std::string channelname)
{
    /*
     *  m_pLCM = new lcm::LCM("udpm://239.255.76.67:7667?ttl=1");
     *  ttl: time to live of transmitted packets
     *  ttl-number changed into 2 or 3
     *  because of PX2 network environment
     */
    m_pLCM = new lcm::LCM("udpm://239.255.76.67:7667?ttl=3");
    mychannelname=channelname;
    m_bInitialized = false;
    isenable=true;
}


inline CLCMSubscriber::CLCMSubscriber(bool _isWatchdog, int _valid, const std::string channelname,void* (*_callMainHander)(const std::string))
{
    //m_pLCM = new lcm::LCM("udpm://239.255.76.67:7667?ttl=1");
    m_pLCM = new lcm::LCM("udpm://239.255.76.67:7667?ttl=3");
    mychannelname=channelname;
    IsValid=false;
    m_bInitialized = false;
    m_valid=_valid;
    m_realcount=_valid;
    m_isWatchdog=_isWatchdog;
    isenable=true;
    callMainHander=_callMainHander;
}

inline CLCMSubscriber::~CLCMSubscriber()
{

}


inline void CLCMSubscriber::run()
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


inline bool CLCMSubscriber::IsInitialized()
{
    return m_bInitialized;
}

inline bool CLCMSubscriber::validmessage()
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
class CLCMHandler_M
{
public:
    CLCMHandler_M();
    void MsgCallback(const lcm::ReceiveBuffer* recvBuf, const std::string& channelName, const T* msg);
    void Get(T &m_result);
    //T m_result;
private:
    T m_buffer;
    std::mutex m_mutex;
};

template<class T>
class CLCMSubscriber_M:public CLCMSubscriber
{
public:
    CLCMSubscriber_M(const std::string channelname);
    CLCMSubscriber_M(bool isWatchdog, int valid,const std::string channelname,void *(*callMainHander)(const std::string channelname));
    ~CLCMSubscriber_M();
    void Send(T &sendmsg);
    virtual void InitializeLCM() override;
    CLCMHandler_M<T> m_lcmHandler;
    /**
     * @brief InitializeLCM_with_low  In order to receive lcm messages in RTE envirenment ,need to set the setQueueCapacity to 1;
     */
    void InitializeLCM_with_setQueueCapacity();
private:

};

//cpp-CLCMHandler_M
template<class T>
CLCMHandler_M<T>::CLCMHandler_M()
{

}

template<class T>
void CLCMHandler_M <T>::MsgCallback(const lcm::ReceiveBuffer *recvBuf, const std::string &channelName, const T *msg)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_buffer=std::move(*msg);
//    m_mutex.lock();
//    m_buffer=*msg;
//    m_mutex.unlock();

    //    if(DEBUG>0){
    //        cout<<"CHANNEL:"<<channelName<<" ("<<GetCurrentTimeString()<<","<<GetLocalTimeStampGapInMillisecond(tmpMicroSecTime)<<"ms)"<<endl;
    //    }
}

template<class T>
void CLCMHandler_M <T>::Get(T &m_result)
{
//    m_mutex.lock();
//    m_result=m_buffer;
//    m_mutex.unlock();
//    std::lock_guard<std::mutex> lock(m_mutex);
    m_result=std::move(m_buffer);
}

//cpp-CLCMSubscriber_M
template<class T>
CLCMSubscriber_M <T>::CLCMSubscriber_M(const std::string channelname):CLCMSubscriber(channelname)
{

}

template<class T>
CLCMSubscriber_M <T>::CLCMSubscriber_M(bool isWatchdog, int valid, const std::string channelname, void *(*callMainHander)(const std::string))
    :CLCMSubscriber(isWatchdog,valid,channelname,callMainHander)
{

}

template<class T>
CLCMSubscriber_M <T>::~CLCMSubscriber_M()
{

}

template<class T>
void CLCMSubscriber_M <T>::Send(T &sendmsg)
{
    m_pLCM->publish(mychannelname,&sendmsg);
}

template<class T>
void CLCMSubscriber_M <T>::InitializeLCM()
{
    if(!m_pLCM->good())
    {
        //PrintErrorInfo("initialize communication error, please check the ethernet connection", __FILE__, __FUNCTION__, __LINE__);
        m_bInitialized = false;
    }

    m_pLCM->subscribe(mychannelname, &CLCMHandler_M<T>::MsgCallback, &m_lcmHandler);
    m_bInitialized = true;
}

template<class T>
void CLCMSubscriber_M<T>::InitializeLCM_with_setQueueCapacity()
{
    if(!m_pLCM->good())
    {
        //PrintErrorInfo("initialize communication error, please check the ethernet connection", __FILE__, __FUNCTION__, __LINE__);
        m_bInitialized = false;
    }

    lcm::Subscription* p= m_pLCM->subscribe(mychannelname, &CLCMHandler_M<T>::MsgCallback, &m_lcmHandler);
    p->setQueueCapacity(1);  // In order to always receive the newest lcm msg;
    m_bInitialized = true;
}

}  // namespace ipc
}  // namespace atd


#endif // MODULES_IPC_LCM_CLCMSUBSCRIBER_H
