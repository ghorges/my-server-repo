#include <iostream>
#include <queue>
#include <sys/epoll.h>
#include <time.h>
#include <signal.h>

using namespace std;
/*
大根堆定义：priority_queue< int >pq
小根堆定义：priority_queue< int ,vector< int >,greater< int > >pq
push() 元素入队
pop() 队首元素出队
top() 取队首元素
empty() 如果队列为空，则返回true（1），否则返回false（0）
size() 返回优先队列中拥有的元素个数
*/
#define TIMEOUT 3000
priority_queue < int >pq;

void handle(int t)
{
    if( t == SIGINT ) {
        pq.push(time( NULL ) + 3);
        cout << "add ok" << endl;
    }
}
int main()
{
    int fd = epoll_create(5);
    struct epoll_event events[10];
    signal(SIGINT,handle);

    time_t start = time( NULL );
    time_t end = time( NULL );
    int timeout = 0;
    while (1) {
        start = time( NULL );
        int nums = epoll_wait(fd, events, 10, timeout);

        timeout -= (end - start) * 1000;
        if(timeout <= 0 || nums == 0) {
            while(1) {
                if(pq.empty()) {
                    timeout = -1;
                    break;
                }
                if (time( NULL ) >= pq.top()) {
                    cout << "一个已出!!" << pq.top() - time( NULL ) << endl;
                    pq.pop();
                } else {
                        timeout = (pq.top() - time( NULL )) * 1000;
                    }
                    break;
                }
            }
        }
    }
    cout << "Hello World!" << endl;
    return 0;
/*

    //这部分是书上代码,可以借鉴
    int timeout = TIMEOUT;
    time_t start = time( NULL );
    time_t end = time( NULL );
    while (1) {
        printf( "the timeout is now %d mil-seconds\n",timeout );
        start = time( NULL );
        int nums = epoll_wait(fd, events, 10, timeout);
        if( (nums < 0) && (errno != EINTR) ) {
            printf( "epoll failure\n" );
            break;
        }
        if( nums == 0 ) {
            timeout = TIMEOUT;
            continue;
        }

        end = time( NULL );
        timeout -= (end - start) * 1000;

        if(timeout <= 0) {
            timeout = TIMEOUT;
        }

    }
    */
}


