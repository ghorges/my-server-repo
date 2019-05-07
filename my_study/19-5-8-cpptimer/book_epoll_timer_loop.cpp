#include <iostream>
#include <time.h>
#include <netinet/in.h>

using namespace std;

#define BUFFER_SIZE 64

struct client_data
{
    sockaddr_in addess;
    int sockfd;
    char buffer[BUFFER_SIZE];
    tw_timer* timer;
};
class tw_timer
{
public:
    tw_timer( int rot,int ts )
        : next( NULL ) ,prev( NULL ),rotation( rot ),time_slot( ts ){}

public:
    int rotation;   //记录定时器在时间轮多少圈后生效
    int time_slot;  //记录定时器属于时间轮上哪个槽
    void (*cb_func) (client_data *);    //定时器回调函数
    client_data* user_data;             //客户数据
    tw_timer* next;
    tw_timer* prev;
};

class time_wheel
{
public:
    time_wheel() : cur_slot( 0 )
    {
        for ( int i = 0; i < N; i++ ) {
            slots[i] = NULL;
        }
    }
    ~time_wheel()
    {
        for ( int i = 0;i < N; ++i ) {
            tw_timer* tmp = slots[i];
            while (tmp) {
                slots[i] = tmp->next;
                delete tmp;
                tmp = slots[i];
            }
        }
    }

    tw_timer* add_timer( int timeout )
    {
        if ( timeout < 0 ) {
            return NULL;
        }
        int ticks = 0;
        if(timeout < SI) {
            ticks = 1;
        }
        else {
            ticks = timeout / SI;
        }
        int rotation = ticks / N;
        int ts = ( cur_slot + ( ticks % N ) ) % N;
        tw_timer* timer = new tw_timer( rotation, ts );
        if( !slots[ts] )
        {
            slots[ts] = timer;
        } else {
            timer->next = slots[ts];
            slots[ts]->prev = timer;
            slots[ts] = timer;
        }
        return timer;
    }

    void del_timer( tw_timer* timer)
    {
        if (!timer) {
            return;
        }
        int ts = timer->time_slot;

        if(timer == slots[ts] ) {
            slots[ts] = slots[ts]->next;
            if (slots[ts]) {
                slots[ts]->prev = NULL;
            }
            delete timer;
        }
        else {
            timer->prev->next = timer->next;
            if ( timer->next )
            {
                timer->next->prev = timer->prev;
            }
            delete timer;
        }
    }

    void tick()
    {
        tw_timer* tmp = slots[cur_slot];
        while ( tmp ) {
            if( tmp->rotation > 0 ) {
                tmp->rotation--;
                tmp = tmp->next;
            } else {
                tmp->cb_func( tmp->user_data );
                if ( tmp == slots[cur_slot] ) {
                    slots[cur_slot] = slots[cur_slot]->next;
                    delete tmp;
                    if ( slots[cur_slot] ) {
                        slots[cur_slot]->prev = NULL;
                    }
                    tmp = slots[cur_slot];
                } else {
                    tmp->prev->next = tmp->next;
                    if ( tmp->next ) {
                        tmp->next->prev = tmp->prev;
                    }
                    tw_timer* tmp2 = tmp->next;
                    delete tmp;
                    tmp = tmp2;
                }
            }
        }
    }

private:
    static const int N = 60;
    static const int SI = 1;
    tw_timer* slots[N];
    int cur_slot;
};
int main()
{
    cout << "Hello World!" << endl;
    return 0;
}