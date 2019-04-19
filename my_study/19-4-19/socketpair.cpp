#include <iostream>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>

using namespace std;

int fd_pipe(int *fd)
{
    return(socketpair(AF_UNIX,SOCK_STREAM,0,fd));
}
int main()
{
    int fd[2];
    if(fd_pipe(fd) < 0)
    {
        cout << "socketpair err:" << endl;
        return 0;
    }
    std::cout << "Hello world" << std::endl;
    return 0;
}

