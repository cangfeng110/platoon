#ifndef _INCLUDE_BASE_UTILS_H_
#define _INCLUDE_BASE_UTILS_H_

#include <unistd.h>
#include <stdint.h>

namespace platoon
{

namespace base
{

int createTimerfd();
int createEventfd();
void readTimerfd(int timerfd);
void setTimerfd(int timerfd, int64_t delayMicrosecond, int64_t intervalMicrosecond);
size_t hashCurrentThreadId();

}
}

#endif // _INCLUDE_BASE_UTILS_H_
