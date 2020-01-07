#ifndef _INCLUDE_BASE_TIMERQUEUE_H_
#define _INCLUDE_BASE_TIMERQUEUE_H_

#include <memory>
#include <set>
#include <map>
#include <vector>

#include "include/base/NonCopyable.h"
#include "include/base/Callbacks.h"
#include "include/base/Timestamp.h"
#include "include/base/Channel.h"
#include "include/base/TimerId.h"

namespace platoon
{

namespace base
{

class EventLoop;

class TimerQueue : public NonCopyable
{
public:
    TimerQueue(EventLoop* loop);
    ~TimerQueue();

    TimerId addTimer(const TimerCallback& cb, int64_t delayMicrosecond, int64_t intervalMicrosecond);

    void cancelTimer(TimerId timerId);

private:

    class Timer;

    void handleRead(TimerId timerId);
    void addTimerInLoop(std::shared_ptr<Timer> timer);
    void cancelTimerInLoop(TimerId timerId);

    typedef std::map<TimerId, std::shared_ptr<Timer>> TimerMap;
    TimerMap _timers;
    EventLoop* _loop;
};

}

}
#endif  // _INCLUDE_BASE_TIMERQUEUE_H_
