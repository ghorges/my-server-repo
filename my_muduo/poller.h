#ifndef POLLER_H
#define POLLER_H

#include <iostream>
#include <functional>
#include <memory>
#include <map>
#include <vector>
#include <sys/epoll.h>
#include <assert.h>
#include "eventloop.h"
#include "channel.h"

using namespace std;

class Channel;
class EventLoop;

class poller : public noncopyable
{
public:
    typedef vector<Channel*> channellist;

    poller();
    ~poller();

    int poll(int timeouts,channellist* activechannels);

    void fillactivechannels(int nums,channellist* activechannels);

    void add(Channel* cha,int events);

    void del(Channel* cha);

    void mod(Channel* cha,int events);

private:
    typedef map<int , Channel*> channelmap;
    typedef vector<struct epoll_event> pollfdlist;

    int epfd_;
    pollfdlist pollfds_;
    channelmap channels_;
};

#endif // POLLER_H
