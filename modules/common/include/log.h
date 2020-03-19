#ifndef MODULES_COMMON_LOG_H_
#define MODULES_COMMON_LOG_H_


#include "easylogging++.h"
#include "rang.hpp"
#include "string.h"


//#define ADEBUG VLOG(4) << "[DEBUG] "
#define ADEBUG LOG(DEBUG)
#define AINFO LOG(INFO)
#define AWARN LOG(WARNING)
#define AERROR LOG(ERROR)
#define AFATAL LOG(FATAL)

// LOG_IF
#define AINFO_IF(cond) LOG_IF(cond,INFO)
#define AERROR_IF(cond) LOG_IF(cond,ERROR)
//#define ACHECK(cond) CHECK(cond)

// LOG_EVERY_N
#define AINFO_EVERY(freq) LOG_EVERY_N(freq,INFO)
#define AWARN_EVERY(freq) LOG_EVERY_N(freq,WARNING)
#define AERROR_EVERY(freq) LOG_EVERY_N(freq,ERROR)

#define AINFO_LOG_DETAIL_EVERY(freq)     AINFO_EVERY(freq)<<"at file: "<<__FILE__<<" function:"<<__FUNCTION__<<" line: "<<__LINE__<<" system error info: "<<std::strerror(errno)<<"|"

#define AINFO_LOG_DETAIL     AINFO<<"at file: "<<__FILE__<<" function:"<<__FUNCTION__<<" line: "<<__LINE__<<" system error info: "<<std::strerror(errno)<<"|"

#define AERROR_LOG_DETAIL_EVERY(freq)     AERROR_EVERY(freq)<<"at file: "<<__FILE__<<" function:"<<__FUNCTION__<<" line: "<<__LINE__<<" system error info: "<<std::strerror(errno)<<"|"

#define AERROR_LOG_DETAIL     AERROR<<"at file: "<<__FILE__<<" function:"<<__FUNCTION__<<" line: "<<__LINE__<<" system error info: "<<std::strerror(errno)<<"|"

#define RETURN_IF_NULL(ptr)               \
    if (ptr == nullptr) {                 \
        AWARN << #ptr << " is nullptr.";  \
        return;                           \
    }

#define RETURN_VAL_IF_NULL(ptr, val)      \
    if (ptr == nullptr) {                 \
        AWARN << #ptr << " is nullptr.";  \
        return val;                       \
    }

#define RETURN_IF(condition)                   \
    if (condition) {                           \
        AWARN << #condition << " is not met."; \
        return;                                \
    }

#define RETURN_VAL_IF(condition, val)          \
    if (condition) {                           \
        AWARN << #condition << " is not met."; \
        return val;                            \
    }

#endif  // MODULES_COMMON_LOG_H_
