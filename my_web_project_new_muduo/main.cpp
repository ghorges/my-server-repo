//Author:ghorges
//This is my muduo

#include <iostream>
#include <memory>
#include <thread>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <assert.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <signal.h>
#include "eventloop.h"
#include "channel.h"
#include "poller.h"
#include "socket.h"

using namespace std;



void output(int i)
{
    cout << i << endl;
}

int main()
{
    chdir("/phpstudy/www/");
    signal(SIGPIPE,SIG_IGN);

    poller p1;
    EventLoop t1(&p1);
    Channel *ch1 = new Channel(&t1);

    //懒汉式单例
    Socket *listen_socket = Socket::getSocket(8888);
    ch1->set_fd(listen_socket->GetFd());

    ch1->setNonblock();
    ch1->setreadcallback(std::bind(&Channel::handleaccept,ch1));
    ch1->setwritecallback(std::bind(&Channel::handlerr,ch1));
    p1.add(ch1,EPOLLIN | EPOLLERR |  EPOLLRDHUP | EPOLLHUP);

    t1.loop();

    cout << "Hello World!" << endl;
    return 0;
}


int main001()
{
    //创建对象即启动
    thread t(output, 10);
    sleep(1);
    cout << "111111111" << endl;
    t.join();
    return 0;

    //detach方式，启动的线程自主在后台运行，当前的代码继续往下执行
    //不等待新线程结束。前面代码所使用的就是这种方式。
    //join方式，等待启动的线程完成，才会继续往下执行
    //假如前面的代码使用这种方式，其输出就会0,1,2,3
    //因为每次都是前一个线程输出完成了才会进行下一个循环，启动下一个新线程。
    for (uint8_t i = 0; i < 4; i++)
    {
        thread t(output, i);
        cout << "111111111" << endl;
        t.join();
        //t.detach();
    }
    return 0;
}
