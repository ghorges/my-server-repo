#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include "noncopyable.h"
#include "channel.h"
#include "poller.h"
#include <iostream>
#include <assert.h>
#include <vector>

using namespace std;
class Channel;
class poller;

class EventLoop : public noncopyable
{
public:
    EventLoop(poller*);
    ~EventLoop();

    poller* get_poller()
    {
        return poller_;
    }
    void loop();

private:
    poller* poller_;
    bool looping_;
    typedef std::vector<Channel*> channellist;

    channellist activechannel;
};

#endif // EVENTLOOP_H
