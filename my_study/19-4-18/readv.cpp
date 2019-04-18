#include <iostream>
#include <sys/uio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

using namespace std;

int main()
{
    int fd = open("www.txt",O_RDWR | O_CREAT);
    if(fd < 0)
    {
        perror("open err:");
        return 0;
    }
    char *c = "123213232132132131225432534\n";
    char *d = "wwwwwwwwwwwwwwwwwwwwwwwwww\n";
    struct iovec iov[10] = {0};
    iov[0].iov_base = c;
    iov[0].iov_len = strlen(c);


    iov[1].iov_base = d;
    iov[1].iov_len = strlen(d);
    
    int i = 2;
    writev(fd,iov,i);
    std::cout << "Hello world" << std::endl;
    return 0;
}

