#include "modules/ipc/include/monitor_log/monitor_global.h"

namespace atd {
namespace ipc {
namespace monitor {

MonitorGlobal::MonitorGlobal() {}

void MonitorGlobal::Init(const MonitorMessageItem::MessageSource &source)
{
//    atd::ipc
    instance()->logger_=new atd::ipc::monitor::MonitorLogger(source);
    instance()->log_buffer_=new atd::ipc::monitor::MonitorLogBuffer(instance()->logger_);
}

MonitorLogBuffer *MonitorGlobal::mlog()
{
    return instance()->log_buffer_;
}

}  // namespace monitor
}  // namespace ipc
}  // namespace atd
