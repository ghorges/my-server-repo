#include "poller.h"

poller::poller() : pollfds_(100)
{
    epfd_ = epoll_create(1);
    assert(epfd_ >= 0);
}

poller::~poller()
{

}

int poller::poll(int timeouts,channellist* activechannels)
{

    int nums = epoll_wait(epfd_,&*pollfds_.begin(),pollfds_.size(),timeouts);
    cout << "epoll_wait ok nums = " << nums << endl;
    if(nums > 0) {
        fillactivechannels(nums,activechannels);
    }
    else if(nums == 0) {
        cout << "time over" << endl;
        return 0;
    }
    else {
        if(errno == EINTR)
            return -1;
        perror("-1:");

    }

}

void poller::fillactivechannels(int nums,channellist* activechannels)
{
    for(pollfdlist::iterator pfd = pollfds_.begin();pfd != pollfds_.end() && nums > 0;++pfd) {
        --nums;
        //int now_sockfd = events[i].data.fd;
        channelmap::const_iterator ch = channels_.find((*pfd).data.fd);
        Channel* channel = ch->second;
        channel->set_revents(static_cast<int>(pfd->events));
        activechannels->push_back(channel);
    }
}

//这是什么操作...
void poller::add(Channel* cha,int events){
    struct epoll_event ev;
    ev.data.fd = cha->fd();
    ev.events = events;
    assert(epoll_ctl(epfd_,EPOLL_CTL_ADD,cha->fd(),&ev) != -1);
    channels_.insert(pair<int , Channel*>(cha->fd(),cha));
}

void poller::del(Channel* cha)
{
    assert(epoll_ctl(epfd_,EPOLL_CTL_DEL,cha->fd(),0) != -1);

    //通过find找到需要删除的map,delete channel后erase channel
    channelmap::iterator ch = channels_.find(cha->fd());

    close(cha->fd());
    //delete channel*
    delete ch->second;

    channels_.erase(ch);
}

void poller::mod(Channel* cha,int events){
    struct epoll_event ev;
    ev.data.fd = cha->fd();
    ev.events = events;
    assert(epoll_ctl(epfd_,EPOLL_CTL_MOD,cha->fd(),&ev) != -1);
}
