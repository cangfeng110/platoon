#ifndef MODULES_COMMON_MONITOR_LOG_MONITOR_LOG_BUFFER_H_
#define MODULES_COMMON_MONITOR_LOG_MONITOR_LOG_BUFFER_H_

#include <string>
#include <utility>
#include <vector>

#include "modules/ipc/include/monitor_log/monitor_logger.h"
#include "modules/ipc/message/protobuf/monitor/monitor_log/monitor_log.pb.h"

/**
 * @namespace atd::ipc::monitor
 * @brief atd::ipc::monitor
 */
namespace atd {
namespace ipc {
namespace monitor {

#define REG_MSG_TYPE(TYPE)                             \
    MonitorLogBuffer &TYPE(const std::string &msg) {   \
    AddMonitorMsgItem(MonitorMessageItem::TYPE, msg);  \
    return *this;                                      \
}                                                    \
    MonitorLogBuffer &TYPE() {                           \
    level_ = MonitorMessageItem::TYPE;                 \
    return *this;                                      \
}

/**
 * class MonitorLogBuffer
 *
 * @brief This class help collect MonitorMessage pb to monitor topic.
 * The messages can be published automatically when the MonitorBuffer object's
 * destructor is called, or can be published by calling function Publish().
 */
class MonitorLogBuffer {
public:
    /**
   * @brief The constructor of MonitorBuffer.
   * @param a Monitor instance pointer;
   */
    explicit MonitorLogBuffer(MonitorLogger *monitor);

    virtual ~MonitorLogBuffer();

    /**
   * @brief print a log trace for the last recorded message.
   */
    void PrintLog();

    /**
   * @brief record an DEBUG L1 type message
   */
    REG_MSG_TYPE(DEBUG1);

    /**
   * @brief record an INFO type message
   */
    REG_MSG_TYPE(INFO);

    /**
   * @brief record a WARN type message
   */
    REG_MSG_TYPE(WARN);

    /**
   * @brief record an ERROR type message
   */
    REG_MSG_TYPE(ERROR);


    /**
   * @brief record a FATAL type message
   */
    REG_MSG_TYPE(FATAL);

    /**
   * @brief Add monitor message with MonitorMessageItem::LogLevel
   * @param log_level defined in modules/common/monitor/proto/monitor.proto
   * @param msg the string to send to monitor
   */
    void AddMonitorMsgItem(const MonitorMessageItem::LogLevel log_level,
                           const std::string &msg);

    /**
   * @brief overload operator << to help join messages
   */
    template <typename T>
    MonitorLogBuffer &operator<<(const T &msg) {
        if (monitor_msg_items_.empty() ||
                monitor_msg_items_.back().first != level_) {
            AddMonitorMsgItem(level_, std::to_string(msg));
        } else {
            monitor_msg_items_.back().second += std::to_string(msg);
        }
        return *this;
    }

    /**
   * @brief overload operator << to help join string messages
   */
    MonitorLogBuffer &operator<<(const std::string &msg);

    /**
   * @brief overload operator << to help join char messages
   */
    MonitorLogBuffer &operator<<(const char *msg);

    /**
   * @brief publish the monitor messages
   */
    void Publish(MonitorMessage &monitor_msg, double cur_timestamp);
    void Clear();

    MonitorLogger *logger_ = nullptr;
private:
    MonitorMessageItem::LogLevel level_ = MonitorMessageItem::INFO;
    std::vector<MessageItem> monitor_msg_items_;


};

}  // namespace monitor
}  // namespace ipc
}  // namespace atd

#endif  // MODULES_COMMON_MONITOR_LOG_MONITOR_LOG_BUFFER_H_
