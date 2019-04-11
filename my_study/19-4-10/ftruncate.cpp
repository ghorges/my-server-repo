#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

using namespace std;
//ftruncate和truncate会直接把这个文件扩大或缩小到你写的数............
//这个配合mmap使用就好了
int main()
{
    int fd = open("aaa.txt",O_CREAT | O_RDWR);
    if(fd == -1)
    {
        perror("open err:");
        return 0;
    }

    if(ftruncate(fd,100000) == -1)
    {
        perror("ftruncate err:");
        return -1;
    }
    std::cout << "Hello world" << std::endl;
    return 0;
}

