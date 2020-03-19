#ifndef MODULES_PERCEPTION_LIB_BASE_TIMER_H_
#define MODULES_PERCEPTION_LIB_BASE_TIMER_H_

#include <chrono>
#include <string>

#include "modules/common/include/macro.h"

namespace atd {
namespace common {

using TimePoint = std::chrono::steady_clock::time_point;

class Timer {
 public:
  Timer() = default;

  // no-thread safe.
  void Start();

  // return the elapsed time,
  // also output msg and time in glog.
  // automatically start a new timer.
  // no-thread safe.
  uint64_t End(const std::string &msg);

 private:
  // in ms.
  TimePoint start_time_;
  TimePoint end_time_;

  DISALLOW_COPY_AND_ASSIGN(Timer);
};

class TimerWrapper {
 public:
  explicit TimerWrapper(const std::string &msg) : msg_(msg) {
    timer_.Start();
  }

  ~TimerWrapper() {
    timer_.End(msg_);
  }

 private:
  Timer timer_;
  std::string msg_;

  DISALLOW_COPY_AND_ASSIGN(TimerWrapper);
};

}  // namespace common
}  // namespace atd

#define PERF_FUNCTION(function_name) \
  atd::common::TimerWrapper _timer_wrapper_(function_name)

#define PERF_BLOCK_START()           \
  atd::common::Timer _timer_; \
  _timer_.Start()

#define PERF_BLOCK_END(msg) _timer_.End(msg)

#endif  // MODULES_PERCEPTION_LIB_BASE_TIMER_H_
