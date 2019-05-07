#include <stdio.h>
#include <sys/epoll.h>
#include <assert.h>
#include <time.h>
#include <signal.h>
//这个是最基本的epoll的timer,这个就是每3s一次,可以用来时间轮
#define TIMEOUT 3000
void handle(int t)
{
    if (t == SIGINT) {
        printf("SIGINT\n");
        sleep(1);
    }
}
int main()
{
    signal(SIGINT,handle);
    time_t timeout = TIMEOUT;
    time_t start = time( NULL );
    time_t end = time( NULL );
    int epollfd = epoll_create( 5 );
    assert(epollfd > 0);

    struct epoll_event events[10];
    bzero( &events, sizeof( events ) );

    while (1) {
        printf("timeout = %d",timeout);
        start = time( NULL );
        int number = epoll_wait( epollfd, events, 10, timeout );
        if(number == 0) {
            printf( "timeout = 0,timeout\n");
            timeout = TIMEOUT;
            continue;
        }

        end = time( NULL );
        timeout -= ( end - start ) * 1000;

        if(timeout <= 0) {
            printf( "timeout < 0,timeout\n");
            timeout = TIMEOUT;
        }
    }

    printf("Hello World!\n");
    return 0;
}
