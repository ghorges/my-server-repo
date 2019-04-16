#include <iostream>
#include <unistd.h>
#include <pthread.h>

//int pthread_spin_lock(pthread_spinlock_t *lock);
//int pthread_spin_trylock(pthread_spinlock_t *lock);
//int pthread_spin_destroy(pthread_spinlock_t *lock);
//int pthread_spin_init(pthread_spinlock_t *lock, int pshared);

using namespace std;
pthread_spinlock_t lock;

void *aaa(void *p)
{
    pthread_spin_lock(&lock);
    cout << "aaaaaaaaaa" << endl;
    pthread_spin_unlock(&lock);
}
int main()
{
    //PTHREAD_PROCESS_PRIVATE和PTHREAD_PROCESS_SHARED
    //看不懂,但是应该是用private
    pthread_spin_init(&lock,PTHREAD_PROCESS_PRIVATE);
    //pthread_spin_init(&lock,PTHREAD_PROCESS_SHARED);
    pthread_t t;
    pthread_create(&t,NULL,aaa,NULL);
    pthread_create(&t,NULL,aaa,NULL);
    pthread_create(&t,NULL,aaa,NULL);
    cout << "hello world" << endl;
    sleep(10);
    return 0;
}

