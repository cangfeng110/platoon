#ifndef _INCLUDE_BASE_POLLER_H_
#define _INCLUDE_BASE_POLLER_H_

#include <vector>
#include <map>

#include "include/base/Timestamp.h"

struct epoll_event;

namespace platoon
{
namespace base
{

class Channel;
class EventLoop;

class Poller
{
public:
    typedef std::vector<Channel*> ChannelList;

    Poller(EventLoop* loop);
    ~Poller();

    Timestamp poll(int timeoutMilisecond, ChannelList* activeChannels);

    void insertChannel(Channel* channel);

    void updateChannel(Channel* channel);

    void removeChannel(Channel* channel);

    bool hasChannel(Channel* channel) const;

private:
    static const int kInitEventVectorSize = 32;

    void fillActiveChannels(int numEvents, ChannelList* activeChannels) const;
    void update(int operation, Channel* channel);

    typedef std::vector<struct epoll_event> EventVector;
    typedef std::map<int, Channel*> ChannelMap;

    EventLoop* _loop;
    int _epollfd;
    EventVector _events;
    ChannelMap  _channels;

};

}
}

#endif  // _INCLUDE_BASE_POLLER_H_
