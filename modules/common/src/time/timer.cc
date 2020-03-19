#include "modules/common/include/time/timer.h"

namespace atd {
namespace common {

using std::string;
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::microseconds;

void Timer::Start() {
  start_time_ = std::chrono::steady_clock::now();
}

uint64_t Timer::End(const string &msg) {
  end_time_ = std::chrono::steady_clock::now();
  uint64_t elapsed_time =
      duration_cast<microseconds>(end_time_ - start_time_).count();

  // << "TIMER " << msg << " elapsed_time: " << elapsed_time << " ms";

  // start new timer.
  start_time_ = end_time_;
  return elapsed_time;
}

}  // namespace common
}  // namespace atd
