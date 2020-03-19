#include "modules/ipc/include/monitor_log/monitor_log_buffer.h"

#include "modules/common/include/Config.h"
// ----- For glog -----
#ifdef CC_X86_64
#include "modules/common/include/log.h"
#endif // CC_X86_64
#include "modules/common/include/log.h"
#ifdef USE_FAKELOG
//#include "modules/common/include/FakeLog.h"
#include "modules/common/include/log.h"
#endif // USE_FAKELOG
// ----- End for glog -----

namespace atd {
namespace ipc {
namespace monitor {

MonitorLogBuffer::MonitorLogBuffer(MonitorLogger *logger) : logger_(logger) {}

void MonitorLogBuffer::PrintLog() {
    if (monitor_msg_items_.empty()) {
        return;
    }
    const auto level = monitor_msg_items_.back().first;
    const auto &msg = monitor_msg_items_.back().second;
    switch (level) {
    case MonitorMessageItem::INFO:
        AINFO << msg;
        break;
    case MonitorMessageItem::WARN:
        AWARN << msg;
        break;
    case MonitorMessageItem::ERROR:
        AERROR << msg;
        break;
    case MonitorMessageItem::FATAL:
        AFATAL << msg;
        break;
    default:
        AERROR << "[unknown monitor level]: " << msg;
    }
}

void MonitorLogBuffer::Publish(MonitorMessage &monitor_msg,double cur_timestamp) {
    if (!monitor_msg_items_.empty() && logger_) {
        logger_->Publish(monitor_msg_items_,monitor_msg,cur_timestamp);
        monitor_msg_items_.clear();
        level_ = MonitorMessageItem::INFO;
    }
}

void MonitorLogBuffer::Clear()
{
    monitor_msg_items_.clear();
}

MonitorLogBuffer &MonitorLogBuffer::operator<<(const std::string &msg) {
    if (monitor_msg_items_.empty() || monitor_msg_items_.back().first != level_) {
        AddMonitorMsgItem(level_, msg);
    } else {
        monitor_msg_items_.back().second += msg;
    }
    return *this;
}

MonitorLogBuffer &MonitorLogBuffer::operator<<(const char *msg) {
    if (msg) {
        std::string msg_str(msg);
        return operator<<(msg_str);
    } else {
        return *this;
    }
}

MonitorLogBuffer::~MonitorLogBuffer() { }

void MonitorLogBuffer::AddMonitorMsgItem(
        const MonitorMessageItem::LogLevel log_level, const std::string &msg) {
    level_ = log_level;
    monitor_msg_items_.push_back(std::make_pair(log_level, msg));
}

}  // namespace monitor
}  // namespace ipc
}  // namespace atd
