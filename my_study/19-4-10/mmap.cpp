#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>


using namespace std;
//void *mmap(void *addr, size_t length, int prot, int flags,
//                             int fd, off_t offset);
int main()
{
    struct stat stats;
    int fd = open("www.txt",0666 | O_RDWR);
    if(fd == -1)
    {
        cout << "open err" << endl;
        return 0;
    }
    int a = stat("www.txt",&stats);
    if(a < 0)
    {
        cout << "stat err" << endl;
        return 0;
    }
    ftruncate(fd,4096);

    char *c = (char *)mmap(NULL,4096,PROT_READ | PROT_WRITE,MAP_SHARED,fd,0);
    
    if(c < 0)
    {
        cout << "mmap err" << endl;
        return 0;
    }
    cout << c << endl;
    c[1] = 'b';
    strcat(c,"1111111111111");
    std::cout << "Hello world" << std::endl;
    return 0;
}

