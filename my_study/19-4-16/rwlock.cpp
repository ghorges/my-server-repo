#include <iostream>
#include <pthread.h>
#include <unistd.h>
//读写锁
using namespace std;

pthread_rwlock_t rw = PTHREAD_RWLOCK_INITIALIZER;

void *aaa(void *p)
{
    pthread_rwlock_rdlock(&rw);
    cout << "111" << endl;

    sleep(2);
    pthread_rwlock_unlock(&rw);
}

void *bbb(void *p) 
{
        pthread_rwlock_wrlock(&rw);
        cout << "222" << endl;

}

int main()
{
    pthread_t t;
    //测试:确实读的进入之后写的不能进入
    /*
    pthread_create(&t,NULL,aaa,NULL);
    pthread_create(&t,NULL,aaa,NULL);
    pthread_create(&t,NULL,bbb,NULL);
    */
    //测试:确实处于读模式锁住状态时候,这个时候一个线程以写的方式去获取锁,读写锁会阻塞随后的读模式请求.
    //避免读模式锁长期占用,而写模式锁一直得不到满足.
    pthread_create(&t,NULL,aaa,NULL);
    sleep(1);
    pthread_create(&t,NULL,bbb,NULL);
    pthread_create(&t,NULL,aaa,NULL);

    while(1)
    {
        sleep(10);
    }
    std::cout << "Hello world" << std::endl;
    return 0;
}

