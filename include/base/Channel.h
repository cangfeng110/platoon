#ifndef _INCLUDE_BASE_CHANNEL_H_
#define _INCLUDE_BASE_CHANNEL_H_

#include <functional>
#include <mutex>
#include <memory>

#include "include/base/Timestamp.h"
#include "include/base/NonCopyable.h"

namespace platoon
{

namespace base
{

class EventLoop;

class Channel : public NonCopyable
{
public:
    typedef std::function<void(Timestamp)> EventCallback;

    Channel(EventLoop* loop, int fd, const std::string& name = "");
    ~Channel();

    // Just like event_data_t defined in <sys/epoll.h>
    union UserData
    {
        uint32_t u32;
        uint64_t u64;
        void*    ptr;
    };

    void setReadCallback(const EventCallback& cb)
    { _readCallback = cb; }

    void setWriteCallback(const EventCallback& cb)
    { _writeCallback = cb; }

    void setCloseCallback(const EventCallback& cb)
    { _closeCallback = cb; }

    void setErrorCallback(const EventCallback& cb)
    { _errorCallback = cb; }

    void enableReading() { _events |= kReadEvent; update(); }

    void disableReading() { _events &= ~kReadEvent; update(); }

    void enableWriting() { _events |= kWriteEvent; update(); }

    void disableWriting() { _events &= ~kWriteEvent; update(); }

    void disableAll() { _events = kNoneEvent; update(); }

    void insert();

    void remove(); // remove will disableAll listening events and remove channel from eventloop

    void handleEvent(Timestamp receiveTime);

    EventLoop* getLoop() { return _loop; }
    const std::string& getName() const { return _name; }
    int  getFd() const { return _fd; }
    int  getEvents() const { return _events; }

    // Used by Poller
    void setTevents(int tevents) { _tevents = tevents; }
    int  getTevents() const { return _tevents; }
    int  getInternalFlag() const { return _internalFlag; }
    void setInternalFlag(int internalFlag) { _internalFlag = internalFlag; }

    UserData getUserData() { return _userData; }
    void setUserData(UserData userData) { _userData = userData; }

//    // std::any in c++17
//    std::any& getUserData() { return _userData; }
//    void setUserData(const std::any& userData) { _userData = userData; }

    bool isNoneEvent() const { return _events == kNoneEvent; }
    bool isWriting() const { return _events & kWriteEvent; }
    bool isReading() const { return _events & kReadEvent; }

private:

    static const int kNoneEvent;
    static const int kReadEvent;
    static const int kWriteEvent;


    void update();

    EventLoop* _loop;
    const std::string _name;
    const int  _fd;
    int        _internalFlag;  // Set by Poller
    int        _events;
    int        _tevents; // Triggered event
    int        _index;

    bool _eventHandling;
    bool _addedToLoop;

    UserData _userData;

//    // std::any in c++17
//    std::any _userData;

    EventCallback _readCallback;
    EventCallback _writeCallback;
    EventCallback _closeCallback;
    EventCallback _errorCallback;
};

}

}

#endif // _INCLUDE_BASE_CHANNEL_H_
