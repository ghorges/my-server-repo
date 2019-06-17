#ifndef SOCKET_H
#define SOCKET_H

#include "noncopyable.h"
#include <pthread.h>
#include <thread>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <assert.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

using namespace std;

class Socket : public noncopyable
{
public:
    //static const Socket& GetSocketFd(short port = 8888);
    static Socket* getSocket(short port);
    int GetFd(){
        return listen_fd;
    }
private:
    Socket(short p);
    static short port;
    static Socket *Instance;
    static int listen_fd;
    static pthread_mutex_t mutex;

};

#endif // SOCKET_H
