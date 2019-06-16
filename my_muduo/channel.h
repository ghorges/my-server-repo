#ifndef CHANNEL_H
#define CHANNEL_H

#include <iostream>
#include <functional>
#include <memory>
#include <map>
#include <vector>
#include <fcntl.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <unistd.h>
#include "buffer.h"
#include "eventloop.h"
#include "poller.h"

using namespace std;

class EventLoop;
class buffer;

class Channel
{
public:
    typedef function<void()> EventCallback;
    Channel(EventLoop *loop);
    ~Channel() {
        cout << "delete channel" << endl;
    }
    void handlevent();
    void setreadcallback(const EventCallback cb)
    {readcallback = cb;}

    void setwritecallback(const EventCallback cb)
    {writecallback = cb;}

    void seterrorcallback(const EventCallback cb)
    {errorcallback = cb;}

    void set_revents()
    {

    }

    const int fd() const{
        return fd_;
    }

    int events() { return events_; }

    void set_revents(int revt) { revents_ = revt;}
    int revents() {return revents_;}
    void set_fd(int fd) {
        fd_ = fd;
    }

    void setNonblock() {
        int oflag = fcntl(fd_,F_GETFL,0);
        assert(fcntl(fd_,F_SETFL,oflag | O_NONBLOCK) >= 0);
    }

    void handleaccept();
    void handleread();
    void handlewrite();
    void handlclose();
    void handlerr();

    void send_data(const char *s, size_t len);

private:
    void updata();

    static const int KNoneevent;
    static const int Kreadevent;
    static const int Kwriteevent;

    EventLoop *loop_;
    int fd_;
    int events_;
    int revents_;
    int index_;


    buffer Readbuffer;

    bool CanWriteBuffer;
    buffer Writebuffer;


    EventCallback readcallback;
    EventCallback writecallback;
    EventCallback errorcallback;

};

#endif // CHANNEL_H
