#include "modules/ipc/include/monitor_log/monitor_logger.h"
#include "modules/common/include/time/time.h"

#include "modules/ipc/include/channel_manager.h"

namespace atd {
namespace ipc {
namespace monitor {

using atd::ipc::ChannelManager;
using atd::common::time::Clock;

void MonitorLogger::setIsSendMonitor(bool send)
{
    is_send_monitor=send;
}

void MonitorLogger::Publish(const std::vector<MessageItem> &messages,MonitorMessage &monitor_msg,double cur_timestamp) const {
    // compose a monitor message
    if (messages.empty()) {
        return;
    }
    monitor_msg.Clear();
    for (const auto &msg_item : messages) {
        MonitorMessageItem *monitor_msg_item = monitor_msg.add_item();
        monitor_msg_item->set_source(source_);
        monitor_msg_item->set_log_level(msg_item.first);
        monitor_msg_item->set_msg(msg_item.second);
    }

    // publish monitor messages
    if(is_send_monitor)
    {
        DoPublish_(&monitor_msg,cur_timestamp);
    }
}

void MonitorLogger::DoPublish(MonitorMessage *message,double cur_timestamp) const {
    //DCHECK(ChannelManager::PROTO_Initialized())<< "AdapterManager must be initialized before using monitor.";
    ChannelManager::GET_Monitor_Adapter()->FillHeader(message,cur_timestamp);
    int fix_size=ChannelManager::GET_Monitor_Adapter()->GetFixsize();
    uint8_t input_array[fix_size];
    Status sendproto=ChannelManager::GET_Monitor_Adapter()->PROTO_TO_ARRAY(message,input_array);
    if(sendproto.ok())
    {
        ChannelManager::SEND_ARRAY_Monitor(input_array);
    }
    else
    {
        atd::ipc::monitor::MonitorMessageItem_MessageSource cur_source;
        if(message->item_size()>0)
        {
            cur_source= message->item(0).source();
        }
        else
        {
            cur_source= atd::ipc::monitor::MonitorMessageItem_MessageSource_UNKNOWN;
        }
        message->mutable_item()->Clear();
        auto* oneitem=message->add_item();
        oneitem->set_source(cur_source);
        oneitem->set_log_level(MonitorMessageItem_LogLevel_ERROR);
        oneitem->set_msg("monitor is out of size");
        //<<"ERROR-OUTPUT:"<<sendproto.ToString()<<std::endl;
        Status sendproto_fix=ChannelManager::GET_Monitor_Adapter()->PROTO_TO_ARRAY(message,input_array);
        if(sendproto_fix.ok())
        {
            ChannelManager::SEND_ARRAY_Monitor(input_array);
        }
    }
}

void MonitorLogger::DoPublish_(MonitorMessage *message, double cur_timestamp) const
{
    atd::ipc::fillHeader<MonitorMessage>(*message,"Monitor",cur_timestamp);
    atd::ipc::sendMessageViaLcm<MonitorMessage>("Monitor",*message);
}

}  // namespace monitor
}  // namespace ipc
}  // namespace atd
