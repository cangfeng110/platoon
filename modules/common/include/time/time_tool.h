//
// Created by nicola on 18-6-7.
//

#ifndef ATD_MASTER_TIME_TOOL_H
#define ATD_MASTER_TIME_TOOL_H


#include <chrono>
#include "modules/common/include/time/time.h"

namespace atd {
namespace common {
namespace time {

    using Duration = std::chrono::nanoseconds;
    using Timestamp = std::chrono::time_point<std::chrono::system_clock, Duration>;
    using nanos = std::chrono::nanoseconds;

//    template <typename PrecisionDuration>
//    int64_t AsInt64(const Duration &duration) {
//        return std::chrono::duration_cast<PrecisionDuration>(duration).count();
//    }

//    template <typename PrecisionDuration>
//    int64_t AsInt64(const Timestamp &timestamp) {
//        return AsInt64<PrecisionDuration>(timestamp.time_since_epoch());
//    }

//    inline double ToSecond(const Timestamp &timestamp) {
//        return static_cast<double>(AsInt64<nanos>(timestamp.time_since_epoch())) * 1e-9;
//    }




    class ClockTool {
    public:
        double NowInSeconds();
        Timestamp Now();

    };

} // namespace time
} // namespace common
} // namespace atd

#endif //ATD_MASTER_TIME_TOOL_H
