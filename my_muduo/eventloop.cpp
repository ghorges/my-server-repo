#include "eventloop.h"

EventLoop::EventLoop(poller* poll) : looping_(false) , poller_(poll)
{

}

EventLoop::~EventLoop()
{

}

void EventLoop::loop()
{
    while(1)
    {
        int re = poller_->poll(-1,&activechannel);
        if(re == -1) {
            //如果re为-1,说明是EINTR
            continue;
        }
        //vector<Channel*>::iterator
        for(int t = 0;t < activechannel.size() && activechannel[t] != NULL;t++) {
            activechannel[t]->handlevent();
            //activechannel.erase(activechannel.begin());
        }
        activechannel.resize(0);
    }
}
