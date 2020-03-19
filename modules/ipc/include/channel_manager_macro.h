#ifndef MODULES_IPC_PROTOBUF_CHANNEL_MANAGER_MACRO_H_
#define MODULES_IPC_PROTOBUF_CHANNEL_MANAGER_MACRO_H_

#include "modules/ipc/include/monitor_log/monitor_global.h"
#include "modules/common/include/status/status.h"

#include "modules/common/include/util/string_util.h"

/**
 * @namespace atd::ipc
 * @brief atd::ipc
 */
namespace atd {
namespace ipc {

using atd::common::util::StrCat;
using atd::common::Status;

//======================================proto define=============================================
/// Macro to prepare all the necessary Channel functions when adding a
/// new input/output. For example when you want to listen to
/// car_status message for your module, you can do
/// REGISTER_Channel(CarStatus) write an Channel class called
/// CarStatusChannel, and call EnableCarStatus(`car_status_topic`,
/// true, `callback`(if there's one)) in ChannelManager.
///
#define REGISTER_CHANNEL_PROTO(name)                                                 \
    public:                                                                       \
    static void Enable##name(const std::string &topic_name, const ChannelConfig &config,const int fix_size) {\
    instance()->InternalEnable##name(topic_name, config,fix_size);                      \
}                                                                            \
    static void SEND_ARRAY_##name(const uint8_t inputmsg[])  {\
    instance()->name##Adapter_->GetSubscriber()->Send_Array(inputmsg,instance()->name##Adapter_->GetFixsize());\
}\
    static void RECV_ARRAY_##name(uint8_t inputmsg[]) {\
    instance()->name##Adapter_->GetSubscriber()->m_lcmHandler.Get_Array(inputmsg);\
}\
    static AdapterOnce<name##Proto>* GET_##name##_Adapter() {\
    return instance()->name##Adapter_;\
}\
    private:              \
    AdapterOnce<name##Proto> * name##Adapter_;\
    void InternalEnable##name(const std::string &topic_name,const ChannelConfig &config,const int fix_size) {\
    name##Adapter_ = new AdapterOnce<name##Proto>(topic_name,config,fix_size);\
    if(config.mode() == ChannelConfig::RECEIVE_ONLY){\
    proto_container_recv.push_back(name##Adapter_->GetSubscriber());\
}\
    if(config.mode() == ChannelConfig::RTE_RECEIVE_ONLY){\
    name##Adapter_->GetSubscriber()->InitializeLCM_with_setQueueCapacity();\
}\
}\

//======================================lcm define=============================================
/// Macro to prepare all the necessary Channel functions when adding a
/// new input/output. For example when you want to listen to
/// car_status message for your module, you can do
/// REGISTER_Channel(CarStatus) write an Channel class called
/// CarStatusChannel, and call EnableCarStatus(`car_status_topic`,
/// true, `callback`(if there's one)) in ChannelManager.
///
#define REGISTER_CHANNEL_LCM(name)\
    public:\
    static void Enable##name(const std::string &topic_name, const int max_delay_millionsecond,                     \
    const ChannelConfig &config) {\
    instance()->InternalEnable##name(topic_name,max_delay_millionsecond, config);\
}\
    static void Get##name(name##Lcm &data) {\
    instance()->InternalGet##name(data);\
}\
    static void Publish##name(name##Lcm &data) {\
    instance()->InternalPublish##name(data);\
}\
    static ChannelConfig &Get##name##Config() {\
    return instance()->name##config_;\
}\
    static bool is##name##Enable() {\
    return instance()->name##Register_->isenable;\
}\
    static bool Receive##name##Once() {\
    return (instance()->name##Register_->m_pLCM->handleTimeout(0)<0);\
}\
    private:              \
    ChannelConfig name##config_;\
    CLCMSubscriber_M<name##Lcm> * name##Register_;\
    void InternalEnable##name(const std::string &topic_name,const int max_delay_millionsecond,const ChannelConfig &config) {\
    if(config.mode() == ChannelConfig::PUBLISH_ONLY){\
    name##Register_ = new CLCMSubscriber_M<name##Lcm>(topic_name);\
}\
    if(config.mode() == ChannelConfig::RECEIVE_ONLY){\
    name##Register_ = new CLCMSubscriber_M<name##Lcm>(false,max_delay_millionsecond,topic_name,NULL);\
    lcm_container_recv.push_back(name##Register_);\
}\
    if(config.mode() == ChannelConfig::RTE_RECEIVE_ONLY){\
    name##Register_ = new CLCMSubscriber_M<name##Lcm>(false,max_delay_millionsecond,topic_name,NULL);\
    name##Register_->InitializeLCM_with_setQueueCapacity();\
}\
    name##config_ = config;\
}\
    void InternalGet##name(name##Lcm &data) {\
    name##Register_->m_lcmHandler.Get(data);\
}\
    void InternalPublish##name(name##Lcm &data) {\
    name##Register_->Send(data);\
}\

#define RECV_MSG(name)\
    int fix_size_##name=ChannelManager::GET_##name##_Adapter()->GetFixsize();\
    uint8_t input_array_##name[fix_size_##name];\
    ChannelManager::RECV_ARRAY_##name(input_array_##name);\
    atd::common::Status name##_getproto=ChannelManager::GET_##name##_Adapter()->ARRAY_TO_PROTO(input_array_##name);\
    if(!name##_getproto.ok()){\
    atd::ipc::monitor::MonitorGlobal::mlog()->ERROR(name##_getproto.ToString());}\

#define SEND_MSG(name,send_proto)\
    int fix_size_##name=ChannelManager::GET_##name##_Adapter()->GetFixsize();\
    uint8_t input_array_##name[fix_size_##name];\
    atd::common::Status name##_sendproto=ChannelManager::GET_##name##_Adapter()->PROTO_TO_ARRAY(&send_proto,input_array_##name);\
    if(name##_sendproto.ok()){\
    ChannelManager::SEND_ARRAY_##name(input_array_##name);\
    atd::ipc::monitor::MonitorGlobal::mlog()->INFO(#name)<<" send OK!";}\
    else{atd::ipc::monitor::MonitorGlobal::mlog()->ERROR(name##_sendproto.ToString());}\

#define RECV_MSG_NOLOG(name)\
    int fix_size_##name=ChannelManager::GET_##name##_Adapter()->GetFixsize();\
    uint8_t input_array_##name[fix_size_##name];\
    ChannelManager::RECV_ARRAY_##name(input_array_##name);\
    atd::common::Status name##_getproto=ChannelManager::GET_##name##_Adapter()->ARRAY_TO_PROTO(input_array_##name);\


//#define RECV_MSG_WIOUT_DECODE(name)\


#define SEND_MSG_NOLOG(name,send_proto)\
    int fix_size_##name=ChannelManager::GET_##name##_Adapter()->GetFixsize();\
    uint8_t input_array_##name[fix_size_##name];\
    atd::common::Status name##_sendproto=ChannelManager::GET_##name##_Adapter()->PROTO_TO_ARRAY(&send_proto,input_array_##name);\
    if(name##_sendproto.ok()){\
    ChannelManager::SEND_ARRAY_##name(input_array_##name);}\


#define MONITOR_DEFINE_STATUS() \
    using atd::common::ErrorCode;\
    using atd::common::Status;\
    using atd::ipc::ChannelManager;\
    using atd::ipc::monitor::MonitorGlobal;\
    static Status init_status;\
    static atd::ipc::MonitorProto monitor_msg;\

#define MONITOR_INIT_SELF(name)\
    MonitorGlobal::Init(name);\
    init_status=Status(atd::common::ErrorCode::OK,"none");\

#define MONITOR_CHECK_INIT_ISVALID()\
    if(!init_status.ok()){\
    MonitorGlobal::mlog()->ERROR(init_status.ToString());\
    MonitorGlobal::mlog()->PrintLog();\
    MonitorGlobal::mlog()->Publish(monitor_msg,0);\
}\

#define MONITOR_INIT(name,command)\
    if(init_status.ok()){\
    Status init_##name = command;\
    if(!init_##name.ok()) {\
        init_status= Status(atd::common::ErrorCode::SYS_FILEREAD_ERROR, init_##name.ToString());\
}}\


#define RTE_RECEIVE_PROTO_WITHOUT_DECODE(name)\
if(ChannelManager::GET_##name##_Adapter()->GetSubscriber()->m_pLCM->handleTimeout(0)<0)\
{\
    atd::ipc::monitor::MonitorGlobal::mlog()->INFO(#name)<<" receive Time out!";\
}\


#define MONITOR_PUBLISH(timestamp)\
    MonitorGlobal::mlog()->Publish(monitor_msg,timestamp);\


//#define MONITOR_PUBLISH(timestamp)\
//    if(ChannelManager::get_is_monitor_send())\
//{MonitorGlobal::mlog()->Publish(monitor_msg,timestamp);}\
//    else{MonitorGlobal::mlog()->Clear();}\

}  // namespace ipc
}  // namespace atd

#endif//MODULES_IPC_PROTOBUF_CHANNEL_MANAGER_MACRO_H_
