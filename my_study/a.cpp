#include <stdio.h>
#include <ncurses.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

pthread_mutex_t MUTEX;  /* mutex for sync display */

#define LOCK pthread_mutex_lock(&MUTEX)
#define UNLOCK pthread_mutex_unlock(&MUTEX)

typedef struct _WIN_struct {
    int startx, starty;
    int height, width;
} WIN;

WIN winTitle;      /* title win */
WIN winMenu;   /* Main menu win */
WIN winNews;       /* win news */

WINDOW *create_newwin(int height, int width, int starty, int startx) {
    WINDOW *local_win;
    local_win = newwin(height, width, starty, startx);
    box(local_win, 0, 0);
    wrefresh(local_win);
    return local_win;
}

char *getTimeNow() {                                                                                                                               
    time_t rawtime;
    struct tm *timeinfo;

       time(&rawtime);
    timeinfo = localtime(&rawtime);
    return asctime(timeinfo);
}

void *threadfunc_title(void *p) {
    WINDOW *windowTitle;
    //LINES*0.25, COLS, 0 , 0);
   int startx,starty,widths,height;
    height = LINES * 0.25;
    widths = COLS;
    starty = 0;
    startx = 0;

    LOCK;
    windowTitle = create_newwin(height, widths, starty, startx);
    UNLOCK;

    /* show title and time */
    wprintw(windowTitle,"11111111");
    //wprintw(windowTitle,"1111111111");
    wrefresh(windowTitle);

    for (;;) {
        //wprintw(windowTitle,"hi here");
        //wrefresh(windowTitle);
        sleep(1);
    }
    }

void *threadfunc_menu(void *p) {
    //LINES*0.75, COLS*0.60, LINES*0.25, 0);
    WINDOW *windowMenu;
    int startx,starty,widths,height;
    height = LINES*0.75;
    widths = COLS * 0.60;
    starty = LINES*0.25;
    startx = 0;

    LOCK;
    windowMenu = create_newwin(height, widths, starty, startx);
    UNLOCK;

    for (;;) {
        /* now do nothing */
        sleep(1);
    }

}

void *threadfunc_news(void *p) {
    //LINES*0.75, COLS*0.40, LINES*0.25, COLS*0.60
    int startx,starty,widths,height;
    height = LINES*0.75;
    widths = COLS * 0.40;
    starty = LINES*0.25;
    startx = COLS * 0.60;
    WINDOW *windowNews;
    LOCK;
    windowNews = create_newwin(height, widths, starty, startx);
    UNLOCK;

    for (;;) {
        sleep(1);
    }
}

void initWin(WIN *p_win, int height, int width, int starty, int startx) {
    p_win->height = height;
    p_win->width = width;
    p_win->starty = starty;
    p_win->startx = startx;
}


int main(int argc, char *argv[])
{
    pthread_t pidTitle;
    pthread_t pidMenu;
    pthread_t pidNews;

    initscr();
    start_color();
    cbreak();
    keypad(stdscr, TRUE);
    noecho();                 
        /* init location */
    initWin(&winTitle, LINES*0.25, COLS, 0 , 0);
    initWin(&winMenu, LINES*0.75, COLS*0.60, LINES*0.25, 0);
    initWin(&winNews, LINES*0.75, COLS*0.40, LINES*0.25, COLS*0.60);

    pthread_create(&pidTitle, NULL, threadfunc_title, NULL);
    pthread_create(&pidMenu, NULL, threadfunc_menu, NULL);
    pthread_create(&pidNews, NULL, threadfunc_news, NULL);

    pthread_join(pidTitle, NULL);
    pthread_join(pidMenu, NULL);
    pthread_join(pidNews, NULL);

    endwin();
    return 0;
}
