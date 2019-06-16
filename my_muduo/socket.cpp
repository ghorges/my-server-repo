#include "socket.h"

Socket* Socket::Instance = 0;
short Socket::port = 8888;
int Socket::listen_fd = 0;
pthread_mutex_t Socket::mutex = PTHREAD_MUTEX_INITIALIZER;

Socket::Socket(short p) : noncopyable()
{
    port = p;
    listen_fd = socket(AF_INET,SOCK_STREAM,0);
    assert(listen_fd > 0);

    int on = 1;
    assert(setsockopt(listen_fd, SOL_SOCKET, SO_REUSEPORT, &on, sizeof(on)) >= 0);

    struct sockaddr_in my_addr;
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(p);
    my_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    assert(::bind(listen_fd,(struct sockaddr*)&my_addr,sizeof(my_addr)) >= 0);
    assert(listen(listen_fd,5) >= 0);
}


//这个前面不用写!!!
//Socket::Socket* Socket::getSocket(short port = 8888)
Socket* Socket::getSocket(short port = 8888)
{
    if(Instance == 0)
    {
        pthread_mutex_lock(&mutex);
        if(Instance == 0)
        {
            Instance = new Socket(port);
        }
        pthread_mutex_unlock(&mutex);
    }
    return Instance;
}
