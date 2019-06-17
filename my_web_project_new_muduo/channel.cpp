#include "channel.h"

const int Channel::KNoneevent = 0;
const int Channel::Kreadevent = EPOLLIN | EPOLLPRI;
const int Channel::Kwriteevent = EPOLLOUT;
//terminate called after throwing an instance of 'std::logic_error' what(): basic_string::_S_construct NULL not valid
//主要原因：对一个空指针进行操作。

//类中 const初始化列表 static const得用这样
Channel::Channel(EventLoop *loop) : loop_(loop),events_(0),revents_(0),
    index_(-1),CanWriteBuffer(0),Readbuffer(),Writebuffer(),
    state_(http_head),record_position(0),file_path("")
{
    char work_path[1000];
    getcwd(work_path,sizeof(work_path));
    cwd = work_path;
    cout << "cwd = " << cwd << endl;
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
    if((revents_ & EPOLLERR) && !(revents_ & EPOLLIN))
    {
        cout << "EPOLLERR" << endl;
        handlerr();
    }
    if(revents_ & EPOLLOUT)
    {
        cout << "EPOLLOUT" << endl;
        handlewrite();
    }

}

void Channel::handleread()
{
    int n = Readbuffer.readFd(fd_);

    if (n > 0) {
        cout << n << endl;
        cout << Readbuffer.c_str() << endl;

        int re = 0;

        do{
            re = gethttp();
            //cout << "re = " << re << endl;
            if(re < 0)
            {
                cout << "读取错误" << endl;
                break;
            }
            if(re == 0)
            {
                break;
            }
            if(re > 0)
            {
                if(re == 2)
                {
                    if(state_ == http_head)
                    {
                        state_ = http_body;
                    } else {
                        state_ = http_head;
                    }
                    Readbuffer.rechange(re);
                    send_file();
                }
                else {
                    string str(Readbuffer.c_str(),re);
                    if(strncmp(str.c_str(),"GET ",strlen("GET ")) == 0)
                    {
                        file_path = cwd;
                        file_path += &str.c_str()[4];
                        bool flag = false;
                        for(string::iterator it = file_path.begin();it != file_path.end();it++)
                        {
                            if(*it == ' ')
                            {
                                flag = true;
                            }
                            if(flag)
                            {
                                *it = 0;
                            }
                        }

                        //改变buffer中readindex的位置
                        cout << "file_path = " << file_path << endl;
                    }
                    Readbuffer.rechange(re);
                }
            }
        }while(re);
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

int Channel::gethttp()
{
    cout << "gethttp" << endl;
    char* buffer_index = Readbuffer.c_str();
    char* write_index = Readbuffer.writeindex_data();

    while(buffer_index + record_position < write_index)
    {
        if(buffer_index[record_position] == '\r') {
            if(buffer_index + record_position + 1 < write_index && buffer_index[record_position + 1] == '\n')
            {
                //因为record_position是从０开始的
                int t = record_position + 2;
                record_position = 0;

                return t;
            }
        }

        if(buffer_index[record_position] == '\n') {
            if(record_position > 0 && buffer_index[record_position - 1] == '\r')
            {
                int t = record_position + 1;
                record_position = 0;

                return t;
            }
            else{
                return -1;
            }

        }
        record_position++;
    }
    return 0;
}

int Channel::send_file()
{
    struct stat st_buf;
    bzero(&st_buf,sizeof(st_buf));
    int st = stat(file_path.c_str(),&st_buf);
    if(S_ISDIR(st_buf.st_mode))
    {
        cout << "是目录文件" << endl;
        dir_page();
    }
    else if(S_ISREG(st_buf.st_mode))
    {
        cout << "是普通文件" << endl;
        reg_page();
    }
    else
    {
        cout << "未知文件类型,即将跳转err界面" << endl;
        err_page();
    }
}

void Channel::dir_page()
{
    DIR *dirs;
    struct dirent *ptr;
    //文件长度
    int file_l = 0;
    char c[10000] = {0};

    string buf("");
    buf += "HTTP/1.1 200 ok\r\n";
    buf += "Content-Type:text/html;charset=utf-8\r\n";
    if((dirs = opendir(file_path.c_str())) == NULL)
    {
        perror("opendir err:");
        return;
    }

    sprintf(c,"<!DOCTYPE html><html><head><title>Dir</title></head><body>");
    file_l = strlen(c);
    while((ptr = readdir(dirs)) != NULL)
    {
        char filepath[1000];
        memset(filepath,0,sizeof(filepath));
        strcpy(filepath,file_path.c_str());
        if(filepath[strlen(filepath) - 1] != '/')
        {
            filepath[strlen(filepath)] = '/';
        }
        strcpy(&filepath[strlen(filepath)],ptr->d_name);
        cout << "文件名" << &filepath[strlen(cwd.c_str())] + 1 << endl;
        cout << filepath << endl;
        struct stat st_buf;
        stat(filepath,&st_buf);
        sprintf(&c[file_l],"<a href=\"%s\">%s</a>     %d<br/>",&filepath[strlen(cwd.c_str())] + 1,ptr->d_name,st_buf.st_size);
        cout << ptr->d_name << endl;
        file_l = strlen(c);
    }
    file_l = strlen(c);
    sprintf(&c[file_l],"</body></html>");
    buf += "Content-Length: ";
    char length_size[100];
    sprintf(length_size,"%d",strlen(c));
    string s_len(length_size,strlen(length_size));
    buf += s_len;
    buf += "\r\n\r\n";
    cout << buf << endl;
    send_data(buf.c_str(),buf.length());

    send_data(c,sizeof(c));

    closedir(dirs);
}


void Channel::reg_page()
{
    int fd = open(file_path.c_str(),O_RDWR);
    struct stat st_buf;
    bzero(&st_buf,sizeof(st_buf));
    int st = fstat(fd,&st_buf);
    if(st < 0)
    {
        return;
    }
    char* c =  (char*)mmap(NULL,st_buf.st_size,PROT_READ,MAP_SHARED,fd,0);

    string buf("");
    buf += "HTTP/1.1 200 ok\r\n";
    char my_style[200] = { 0 };
    change_style(file_path.c_str(),my_style);
    buf += "Content-Type:";
    buf += my_style;
    buf += "\r\n";
    buf += "Content-Length: ";
    char length_size[100];
    sprintf(length_size,"%d",st_buf.st_size);
    string s_len(length_size,strlen(length_size));
    buf += s_len;
    buf += "\r\n\r\n";

    cout << buf << endl;
    send_data(buf.c_str(),buf.size());
    send_data(c,st_buf.st_size);

    munmap(c,st_buf.st_size);
}

void Channel::err_page()
{
    file_path = "404.html";

    reg_page();

}

void Channel::change_style(const char *path,char *my_style)
{
    char c[100];
    int i = 0;
    for(i = 0;i < 100;i++)
    {
        if(path[i] == '.')
        {
            i++;
            break;
        }
    }
    strcpy(c,&path[i]);
    cout << c << endl;
    if(strcmp(c,"html") == 0)
    {
        strcpy(my_style," text/html; charset=utf-8");
        cout << "c is html" << endl;
    }
    else if(strcmp(c,"mp3") == 0)
    {
        strcpy(my_style,"audio/mp3");
    }
    else if(strcmp(c,"jpg") == 0)
    {
        strcpy(my_style,"image/jpeg");
    }
    else
    {
        strcpy(my_style,"application/octet-stream");
    }
}
