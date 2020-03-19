#ifndef MODULES_COMMON_MONITOR_LOG_MONITOR_LOGGER_H_
#define MODULES_COMMON_MONITOR_LOG_MONITOR_LOGGER_H_

#include <string>
#include <utility>
#include <vector>

#include "modules/ipc/message/protobuf/monitor/monitor_log/monitor_log.pb.h"
//#include "modules/ipc/include/TemplateBasedIPC/channelmanager_simple.h"

/**
 * @namespace atd::common::monitor
 * @brief atd::common::monitor
 */
namespace atd {
namespace ipc {
namespace monitor {

using MessageItem = std::pair<MonitorMessageItem::LogLevel, std::string>;

/**
 * class MonitorLogger
 *
 * @brief This class help collect and publish MonitorMessage pb to monitor
 * topic. A module who wants to publish message can use macro
 * `MONITOR(log_level, log_msg)` to record messages, and call
 * Publish to broadcast the message to other modules.
 */
class MonitorLogger {
 public:
  /**
   * @brief Construct the monitor with the source of the monitor messages. The
   * source is usually the module name who publish the monitor messages.
   * @param source the source of the monitor messages.
   */
  explicit MonitorLogger(const MonitorMessageItem::MessageSource &source)
      : source_(source),is_send_monitor(false) {}
  virtual ~MonitorLogger() = default;

   void setIsSendMonitor(bool send);

  /**
   * @brief Publish the messages.
   * @param messages a list of messages for
   */
  virtual void Publish(const std::vector<MessageItem> &messages, MonitorMessage &monitor_msg, double cur_timestamp) const;

 private:
    virtual void DoPublish(MonitorMessage *message, double cur_timestamp) const;
    virtual void DoPublish_(MonitorMessage *message, double cur_timestamp) const;

  MonitorMessageItem::MessageSource source_;

  bool is_send_monitor;
};

}  // namespace monitor
}  // namespace ipc
}  // namespace atd

#endif  // MODULES_COMMON_MONITOR_LOG_MONITOR_LOGGER_H_
