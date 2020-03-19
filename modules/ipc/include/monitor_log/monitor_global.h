#ifndef MODULES_COMMON_MONITOR_LOG_MONITOR_GLOBAL_H_
#define MODULES_COMMON_MONITOR_LOG_MONITOR_GLOBAL_H_


#include "modules/common/include/macro.h"
#include "modules/ipc/include/monitor_log/monitor_log_buffer.h"

/**
 * @namespace atd::ipc::monitor
 * @brief atd::ipc::monitor
 */
namespace atd {
namespace ipc {
namespace monitor {

class MonitorGlobal {
public:
    static void Init(const MonitorMessageItem::MessageSource &source);
    static atd::ipc::monitor::MonitorLogBuffer* mlog();

private:

    atd::ipc::monitor::MonitorLogger* logger_;
    atd::ipc::monitor::MonitorLogBuffer* log_buffer_;

    DECLARE_SINGLETON(MonitorGlobal);

};

}  // namespace monitor
}  // namespace ipc
}  // namespace atd

#endif  // MODULES_COMMON_MONITOR_LOG_MONITOR_GLOBAL_H_
