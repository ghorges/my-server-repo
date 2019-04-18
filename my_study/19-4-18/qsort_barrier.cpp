#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/times.h>

using namespace std;

int *c = 0;
pthread_barrier_t m;


int comp(const void *a1,const void *a2)
{
    int b1 = *(int *)a1;
    int b2 = *(int *)a2;

    if(b1 == b2)
        return 0;
    else if(b1 < b2)
        return -1;
    else
        return 1;
}

void *aaa(void *p)
{

    qsort(p,100000,sizeof(int),comp);
    pthread_barrier_wait(&m);
}
int main()
{
    pthread_barrier_init(&m,NULL,11);
    c = (int *)malloc(1000000 * 4);
    if(c == NULL)
    {
        cout << "melloc err" << endl;
        return 0;
    }
    int t;
    cout << "请输入您需要对多少个数排序:" << endl;
    cin >> t;
    for(int i = 0;i < t;i++)
    {
        c[i] = rand();
    }
    

    
    for(int i = 0;i < 10;i++)
    {
        pthread_t t;
        pthread_create(&t,NULL,aaa,(void *)(c + i * 100000));
    }

    pthread_barrier_wait(&m);
    cout << "等待完成!" << endl;
    qsort(c,t,sizeof(int),comp);






    int fd = open("data.txt",O_RDWR | O_CREAT);
    if(fd < 0)
    {
        perror("open err:");
        return 0;
    }
    ftruncate(fd,100 * 1024 * 1024);
    char *aaa = (char *)mmap(NULL,100 * 1024 * 1024,PROT_READ | PROT_WRITE,MAP_SHARED,fd,0);
    int sum = 0;
    if(aaa == NULL)
    {
        perror("mmap err:");
        return 0;
    }
    memset(aaa,0,100 * 1024 * 1024);
    for(int i = 0;i < t;i++)
    {
        char string1[25];
        bzero(string1,25);
        sprintf(string1, "%d\n", c[i]);
        strcat(aaa,string1);
    }
    close(fd);
    struct tms buf;
    times(&buf);
    cout << "user cpu time" << buf.tms_utime << endl;
    cout << "system cpu time" << buf.tms_stime << endl;





    std::cout << "Hello world" << std::endl;
    return 0;
}
