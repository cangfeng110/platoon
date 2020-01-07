#ifndef _INCLUDE_BASE_EVENTLOOP_H_
#define _INCLUDE_BASE_EVENTLOOP_H_

#include <vector>
#include <memory>
#include <mutex>
#include <thread>
#include <string>
#include <functional>

#include "include/base/NonCopyable.h"
#include "include/base/Timestamp.h"
#include "include/base/TimerId.h"
#include "include/base/Callbacks.h"

namespace platoon
{

namespace base
{

class Channel;
class Poller;
class TimerQueue;

class EventLoop : public NonCopyable
{
public:
    typedef std::function<void()> Task;

    EventLoop(const std::string& name = "");

    ~EventLoop();

    void loop();

    void quit();

    void runInLoop(const Task& cb);

    void queueInLoop(const Task& cb);

    size_t queueSize() const;

    Timestamp pollReturnTime() const { return _pollReturnTime; }

    const std::string& name() { return _name; }

    static EventLoop* getEventLoopOfCurrentThread();

    TimerId runAt(const Timestamp& time, const TimerCallback& cb);

    TimerId runAfter(int64_t delayMiliseconds, const TimerCallback& cb);

    TimerId runEvery(int64_t intervalMiliseconds, const TimerCallback& cb);

    void cancel(TimerId timerId);

    void wakeup();

    void insertChannel(Channel* channel);

    void updateChannel(Channel* channel);

    void removeChannel(Channel* channel);

    bool hasChannel(Channel* channel);

    void assertInLoopThread()
    {
        if (!isInLoopThread()) {
            abortNotInLoopThread();
        }
    }

    bool isInLoopThread() const { return _threadId == std::this_thread::get_id(); }

    bool eventHandling() const { return _eventHandling; }

private:

    void abortNotInLoopThread();
    void handleRead();
    void doPendingTasks();

    typedef std::vector<Channel*> ChannelList;

    // Atomic ?
    bool _looping;
    bool _quit;
    bool _eventHandling;
    bool _callingPendingTasks;

    const std::thread::id _threadId;
    const size_t _hashedThreadId;

    Timestamp _pollReturnTime;
    std::unique_ptr<Poller> _poller;
    std::unique_ptr<TimerQueue> _timerQueue;

    int _wakeupFd;
    std::unique_ptr<Channel> _wakeupChannel;

    ChannelList _activeChannels;
    Channel* _currentActiveChannel;

    mutable std::mutex _mutex;
    std::vector<Task> _pendingTasks; // Guarded by _mutex

    const std::string _name;

    static const int kPollTimeMilisecond;
};

}

}
#endif // _INCLUDE_BASE_EVENTLOOP_H_
