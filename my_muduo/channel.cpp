#include "channel.h"

const int Channel::KNoneevent = 0;
const int Channel::Kreadevent = EPOLLIN | EPOLLPRI;
const int Channel::Kwriteevent = EPOLLOUT;
//类中 const初始化列表 static const得用这样
Channel::Channel(EventLoop *loop) : loop_(loop),events_(0),revents_(0),index_(-1),CanWriteBuffer(0),Readbuffer(),Writebuffer()
{

}

void Channel::handleaccept()
{
    struct sockaddr_in addr;
    socklen_t t;
    int fd = accept(fd_, (struct sockaddr*)&addr,&t);
    assert(fd > 0);
    Channel* new_channel = new Channel(loop_);
    new_channel->set_fd(fd);
    new_channel->setNonblock();
    new_channel->setreadcallback(std::bind(&Channel::handleread,new_channel));
    //p->setwritecallback();
    //p->seterrorcallback();

    loop_->get_poller()->add(new_channel,EPOLLIN | EPOLLERR /*| EPOLLOUT*/ | EPOLLHUP | EPOLLRDHUP);
}

void Channel::handlerr()
{
    cout << "err fd_ = " << fd_ << endl;
    loop_->get_poller()->del(this);
}

void Channel::handlevent()
{
    if(revents_ & EPOLLIN)
    {
        cout << "EPOLLIN" << endl;
        readcallback();
    }
    if((revents_ & EPOLLHUP || revents_ & EPOLLRDHUP) && !(revents_ & EPOLLIN))
    {
        cout << "EPOLLHUP" << endl;
        handlclose();
    }
    if(revents_ & EPOLLERR)
    {
        cout << "EPOLLERR" << endl;
        handlerr();
    }
    if(revents_ & EPOLLOUT)
    {
        cout << "EPOLLOUT" << endl;

    }

}

void Channel::handleread()
{
    int n = Readbuffer.readFd(fd_);

    if (n > 0) {
        cout << n << endl;
        cout << Readbuffer.c_str() << endl;


        //明天问问朱，这块我确实不知道应该是我处理还是hup处理
    } else if (n == 0) {
        handlclose();
    } else {
        if(errno == EINPROGRESS || errno == EINTR) {
            return;
        }
        handlerr();
    }

}

void Channel::handlewrite()
{
    send_data("",0);
}

void Channel::handlclose()
{
    cout << "连接断开 fd = " << fd_ << endl;
    loop_->get_poller()->del(this);
}


void Channel::send_data(const char *s, size_t len)
{
    if(len < 0)
    {
        return;
    }
    if(CanWriteBuffer == true || Writebuffer.readableBytes() != 0)
    {
        Writebuffer.append(s,len);
        int w_size = Writebuffer.readableBytes();
        int re = send(fd_,Writebuffer.peek(),w_size,0);
        if(re > 0) {
            if (re < w_size) {
                Writebuffer.rechange(re);
                Writebuffer.prependableBytes();
            }
            if (re == w_size) {
                //说明消息发送完毕
                CanWriteBuffer = false;
                loop_->get_poller()->mod(this,revents_ &~ EPOLLOUT);
            }
        }

        else if (re == 0) {
            handlclose();
        }
        else {
            if(errno == EINTR || errno == EINPROGRESS)
                return;
            handlerr();
        }

    } else {

        int re = send(fd_,s,len,0);
        if (re > 0) {
            if (re < len) {
                Writebuffer.append(&s[re],len - re);
                loop_->get_poller()->mod(this,revents() | EPOLLOUT);
            }
            if (re == len) {
                //说明消息发送完毕
                CanWriteBuffer = false;
            }
        }
        else if (re == 0) {
            handlclose();
        }
        else {
            if(errno == EINTR || errno == EINPROGRESS)
                return;
            handlerr();
        }
    }
}
