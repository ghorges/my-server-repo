#include <iostream>
#include <signal.h>
#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

using namespace std;
void *ccc(void *p)
{
    cout << "88" << endl;
    //exit();
    sigset_t mask;
    sigaddset(&mask,SIGINT);
    int signo;
    cout << "zixiancheng" << endl;
    sigwait(&mask,&signo);
    cout << "sigwait-----" << endl;
    switch(signo){
    case SIGINT:
        cout << "SIGALRM11111111111" << endl;
        break;
    default:
        cout << "whitch sig??" << endl;
    }
    while(1)
    {
        printf("pth``````\n");
    }
}
void aaa(int i)
{
    cout << "sigint" << endl;
}
int main()
{
    signal(SIGINT,aaa);
    pthread_t t;
    pthread_create(&t,NULL,ccc,NULL);
    /*
    if(pthread_kill(t,SIGINT) == 0)
    {
        cout << "pthread_kill succ" << endl;
    }
    */
    while(1)
    sleep(10);
    std::cout << "Hello world" << std::endl;
    return 0;
}
