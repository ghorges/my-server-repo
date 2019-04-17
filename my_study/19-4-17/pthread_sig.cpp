#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>

using namespace std;

void *aaa(void *p)
{
    cout << "aaa come in" << endl;
    int a = 1/0;
    cout << "我退出了" << endl;
}
void bbb(int t)
{
    cout << "float err" << endl;
    pthread_cancel(pthread_self());
    sleep(2);
}
int main()
{
    signal(SIGFPE,bbb);
    pthread_t t;
    pthread_create(&t,NULL,aaa,NULL);
    while(1)
    {
        //int t = sleep(10);
        //if(t)
        //cout << "wark up" << endl;
        
        int t = sleep(3);
        if(t)
        cout << "wark up" << endl;
    }
    std::cout << "Hello world" << std::endl;
    return 0;
}

