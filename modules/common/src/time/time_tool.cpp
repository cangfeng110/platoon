//
// Created by nicola on 18-6-7.
//

#include "modules/common/include/time/time_tool.h"

namespace atd {
namespace common {
namespace time {


    double ClockTool::NowInSeconds() {
        return ToSecond(Now());
    }

    Timestamp ClockTool::Now() {
        return std::chrono::time_point_cast<Duration>(
                std::chrono::system_clock::now());
    }

} // namespace time
} // namespace common
} // namespace atd