#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <queue>
#include <pthread.h>
#include <vector>
#include <aio.h>
#include <errno.h>
#include <dirent.h>

//最少线程数
#define MMin 15
//最大线程数
#define MMax 100

using namespace std;
//定义了一个可以指向void *(void *)函数的数据类型
typedef  void *(*typ_thread)(void *);

typedef struct{
    typ_thread p;
    void *q;
}typ_str_thread;
//queue <typ_thread> thread_tast_que;
//vector <pthread_t> thr_tid;

void *thread_run(void *);
void *r_thread_run(void *);
class my_thread
{
    public:
        //对当前工作的那个数量加锁
        pthread_mutex_t mutex_busy_num = PTHREAD_MUTEX_INITIALIZER;
        //生产者消费者的条件变量，是队列中的
        pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
        //条件变量，没有任务时，等待
        pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
        //普通线程
        //vector <pthread_t> tid_num(100);
        pthread_t tid_num[100];
        //管理者线程
        pthread_t r_tid;

        //当前存活的
        int live_thr_num;
        //当前工作的
        int busy_thr_num;
        //多出来要杀死的
        int wait_thr_num;

        //任务队列
        queue <typ_str_thread> tast;
    
        //线程池是关闭还是开启
        bool shutdown;
public:
    my_thread(void)
    {
        live_thr_num = MMin;
        busy_thr_num = 0;
        wait_thr_num = 0;
        
        shutdown = false;

        //创建初始线程
        for(int i = 0;i < MMin;i++)
        //for(int i = 0;i < 1;i++)
        {
            if(pthread_create(&tid_num[i],NULL,thread_run,(void *)this) < 0)
            {
                perror("create err:");
                exit(-1);
            }
        }
        if(pthread_create(&r_tid,NULL,r_thread_run,(void *)this) < 0)
        {
            perror("create err:");
            exit(-1);
        }
    }
    //新来的任务加入队列
    void thread_add(typ_thread p,void *q)
    {
        typ_str_thread t1;
        t1.p = p;
        t1.q = q;
        pthread_mutex_lock(&mutex1);
        this->tast.push(t1);
        cout << "成功将新来的任务加入队列--------------------------------"  << endl;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex1);
    }
};
void *thread_run(void *p)
{
    //脱离控制
    pthread_detach(pthread_self());
    my_thread *q = (my_thread*)p;
    cout << "我是普通线程"  <<  pthread_self() << endl;
    while(1)
    {
        pthread_mutex_lock(&q->mutex1);
        while(q->tast.empty()){
            pthread_cond_wait(&q->cond,&q->mutex1);
            cout << "我被唤醒了" << endl;
        if(q->wait_thr_num > 0)
        {
            cout << "自杀小队，激活" << endl;
            q->wait_thr_num--;
            pthread_mutex_unlock(&q->mutex1);
            pthread_exit(NULL);
        }
        }
        //取出任务
        typ_str_thread ev_tast = q->tast.front();
        q->tast.pop();
        //立刻释放锁
        pthread_mutex_unlock(&q->mutex1);

        pthread_mutex_lock(&q->mutex_busy_num);
        q->busy_thr_num++;
        pthread_mutex_unlock(&q->mutex_busy_num);
        //执行
        ev_tast.p(ev_tast.q);

        pthread_mutex_lock(&q->mutex_busy_num);
        q->busy_thr_num--;
        pthread_mutex_unlock(&q->mutex_busy_num);
    } 
}
void *r_thread_run(void *p)
{
    pthread_detach(pthread_self());
    my_thread *q = (my_thread*)p;
    int sum = 0;
    while(1)
    {
        cout << "我是管理者线程" << ++sum;
        cout <<  "   " << q->live_thr_num << "   " << q->busy_thr_num << endl;
        if(((q->live_thr_num - q->busy_thr_num) < 10) && (q->live_thr_num < MMax))
        {
            for(int i = 0;i < MMin;i++)
            {   
                pthread_t t;
                if(pthread_create(&t,NULL,thread_run,p) < 0)
                {   
                    perror("create err:");
                    exit(-1);
                }
            }
            q->live_thr_num += MMin;
            cout << "建立一些线程" << endl;
        }

         if((q->live_thr_num - q->busy_thr_num) > 20 && q->live_thr_num > MMin)
         {
            q->wait_thr_num = 10;
            for(int i = 0;i < 10;i++)
            {
                pthread_cond_signal(&q->cond);
            }
            q->live_thr_num -= 10;
            cout << "清除一些线程" << endl;
         }
         sleep(10);
    }
}
//异步读取文件
//
//第三个参数是文件长度
struct aiocb *my_aio(char *p,char *pos,int lth)
{
    struct aiocb *aios = (aiocb *)malloc(sizeof(struct aiocb));
    memset(aios,0,sizeof(aios));
    aios->aio_fildes = open(p,O_RDONLY);
    //open("error.html",O_RDONLY);
    if(aios->aio_fildes < 0)
    {
        perror("open err:");
        return NULL;
    }
    aios->aio_offset = 0;
    //aios->aio_reqprio = 0;
    aios->aio_buf = (void *)pos;
    aios->aio_nbytes = lth;
    aios->aio_sigevent.sigev_notify = SIGEV_NONE;

    if(aio_read(aios) < 0)
    {
        if(errno == EAGAIN)
        {
            cout << "EAGAIN" << endl;
        }
        if(errno == EBADF)
        {
            cout << "EBADF" << endl;
        }
        if(errno == EINVAL)
        {
            cout << "EINVAL" << endl;
        }
        if(errno == ENOSYS)
        {
            cout << "ENOSYS" << endl;
        }
        if(errno == EOVERFLOW)
        {
            cout << "EOVERFLOW" << endl;
        }
        perror("aid_read err:");
        return NULL;
    }
 
    return aios;
}
//错误界面，我设置跳转到GitHub的404上
void err_page(int fd)
{
    //这里就体现了为什么异步快
    char c[1000];
    struct aiocb *aios = my_aio("error.html",c,1000);
    char buf[1024];
    sprintf(buf,"HTTP/1.1 301 ok\r\n");
    write(fd,buf,strlen(buf));
    memset(buf,0,sizeof(buf));
    //sprintf(buf,"Content-Type:text/html;charset=utf-8\r\n");
    sprintf(buf,"Content-Type:text/html;charset=utf-8\r\n");
    write(fd,buf,strlen(buf));
    memset(buf,0,sizeof(buf));
    sprintf(buf,"Content-Length: 185\r\n\r\n");
    write(fd,buf,strlen(buf));
    
    //没法用信号，只能等会
	while(1)
    {
        int t = aio_error(aios);
        int flag = 0;
        switch(t)
        {
        case EINPROGRESS:
            cout << "请求尚未完成" << endl;
            break;
        case ECANCELED:
            cout << "请求取消了"  << endl;
            break;
        case -1:
            perror("aio err:");
            return;
        case 0:
            cout << "请求成功" << endl;
            flag = 1;
            break;
        }
        usleep(10); 
        if(flag)
            break;
    }
    int aio_sum = aio_return(aios);
    cout << aio_sum << endl;
	cout << strlen(c) << endl;
    cout << c << endl;
    free(aios);
    //char c[1000];
    //my_aio("aaa.html",c);
    write(fd,c,strlen(c));
}
void change_style(char *path,char *my_style)
{
    char c[100];
    int i = 0;
    for(i = 0;i < 100;i++)
    {
        if(path[i] == '.')
        {
            i++;
            break;
        }
    }
    strcpy(c,&path[i]);
    cout << c << endl;
    if(strcmp(c,"html") == 0)
    {
        strcpy(my_style," text/html; charset=utf-8");
        cout << "c is html" << endl;
    }
    else if(strcmp(c,"mp3") == 0)
    {
        strcpy(my_style,"audio/mp3");
    }
    else if(strcmp(c,"jpg") == 0)
    {
        strcpy(my_style,"image/jpeg");
    }
    else
    {
        strcpy(my_style,"application/octet-stream");
    }
}
void reg_page(int fd,char *path)
{
    //这里就体现了为什么异步快
    struct stat st_buf;
    stat(path,&st_buf);
    char *c = (char *)calloc(st_buf.st_size + 10,1);
    struct aiocb *aios = my_aio(path,c,st_buf.st_size);
    if(aios == NULL)
    {
        return;
    }
    char buf[1024];
    sprintf(buf,"HTTP/1.1 200 ok\r\n");
    write(fd,buf,strlen(buf));
    memset(buf,0,sizeof(buf));
    //sprintf(buf,"Content-Type:text/html\r\n");
    
    //转变格式
    char my_style[100];
    bzero(&my_style,sizeof(my_style));
    change_style(path,my_style);

    sprintf(buf,"Content-Type:%s\r\n",my_style);
    //sprintf(buf,"Content-Type:application/octet-stream\r\n");
    cout << buf << endl;
    write(fd,buf,strlen(buf));
    memset(buf,0,sizeof(buf));
    
    //没法用信号，只能等会
	while(1)
    {
        int t = aio_error(aios);
        int flag = 0;
        switch(t)
        {
        case EINPROGRESS:
            cout << "请求尚未完成" << endl;
            break;
        case ECANCELED:
            cout << "请求取消了"  << endl;
            break;
        case -1:
            perror("aio err:");
            return;
        case 0:
            cout << "请求成功" << endl;
            flag = 1;
            break;
        }
        usleep(10); 
        if(flag)
            break;
    }
    int aio_sum = aio_return(aios);
    cout << "长度为：" << aio_sum << endl;
	cout << strlen(c) << endl;
    cout << c << endl;
    sprintf(buf,"Content-Length:%d\r\n\r\n",aio_sum);
    cout << buf << endl;
    write(fd,buf,strlen(buf));
    //char c[1000];
    //my_aio("aaa.html",c);

    //这里感觉是不是传的太快，客户端没收到
    //write(fd,c,aio_sum);
    int ret = aio_sum;
    for(int i = 0;i < aio_sum;i+=1000)
    {
        if(ret < 1000)
        write(fd,&c[i],ret);
        else
        write(fd,&c[i],1000);
        ret -= 1000;
    }
    cout << "执行成功" << endl;
    free(aios);
    free(c);
}

void dir_page(int fd,char *path)
{
    printf("`````````````````````=%s\n",&path[13]);
    DIR *dirs;
    struct dirent *ptr;
    //文件长度
    int file_l = 0;
    char c[10000] = {0};

    char buf[1024];
    sprintf(buf,"HTTP/1.1 200 ok\r\n");
    write(fd,buf,strlen(buf));
    memset(buf,0,sizeof(buf));
    sprintf(buf,"Content-Type:text/html;charset=utf-8\r\n");
    write(fd,buf,strlen(buf));
    memset(buf,0,sizeof(buf));
    if((dirs = opendir(path)) == NULL)
    {
        perror("opendir err:");
        return;
    }

    sprintf(c,"<!DOCTYPE html><html><head><title>Dir</title></head><body>");
    file_l = strlen(c);
    while((ptr = readdir(dirs)) != NULL)
    {
        char filepath[100];
        memset(filepath,0,sizeof(filepath));
        strcpy(filepath,&path[14]);
        strcpy(&filepath[strlen(filepath)],ptr->d_name);
        cout << "文件名" << filepath << endl;
        struct stat st_buf;
        stat(filepath,&st_buf);
        sprintf(&c[file_l],"<a href=\"%s\">%s</a>     %d<br/>",filepath,ptr->d_name,st_buf.st_size);
        cout << ptr->d_name << endl;
        file_l = strlen(c);
    }
    file_l = strlen(c);
    sprintf(&c[file_l],"</body></html>");
    sprintf(buf,"Content-Length: %d\r\n\r\n",strlen(c));                                
    write(fd,buf,strlen(buf));
    
    write(fd,c,strlen(c));

    closedir(dirs);
    chdir("/phpstudy/www");
}

void *p(void *a)
{
    cout << "进入执行命令" << endl;
    char *t = (char *)a;
    int mfd = 0;
    for(int i = 0;i < 2048;i++)
    {
        if(t == 0x0)
            mfd = i;
        t--;
    }
    printf("t = %d,mfd = %d\n",t,mfd);
    //cout << "成功执行函数" << t << endl;
    char c[2048];
    memset(c,0,sizeof(c));
    int m = recv(mfd,c,2048,0);
    if(m == 0)
    {
        cout << "连接断开" << endl;
    }
    if(m < 0)
    {
        cout << "连接错误" << endl;
    }
    if(c[0] != 'G' && c[1] != 'E' && c[2] != 'T')
    {
        return NULL;
    }
    cout << c << endl;

    char my_path[100];
    memset(my_path,0,sizeof(my_path));
    //sprintf(my_path,"/phpstudy/www%s",&c[4]);
    strcpy(my_path,"/phpstudy/www");
    int l = strlen(my_path);
    for(int i = 4;i < 100;i++)
    {
        if(c[i] == ' ')
        {
            break;
        }
        my_path[l++] = c[i];
    }
    cout << "my_path = " << my_path << endl;
    
    struct stat st_buf;
    bzero(&st_buf,sizeof(st_buf));
    int st = stat(my_path,&st_buf);
    /*
    if(st < 0)
    {
        perror("stat err:");
        return NULL;
    }
    */
    if(S_ISDIR(st_buf.st_mode))
    {
        cout << "是目录文件" << endl;
        if(my_path[strlen(my_path) - 1] != '/')
        {
            my_path[strlen(my_path)] = '/';
        }
        dir_page(mfd,my_path);
    }
    else if(S_ISREG(st_buf.st_mode))
    {
        cout << "是普通文件" << endl;
        reg_page(mfd,my_path);
    }
    else
    {
        cout << "未知文件类型,即将跳转err界面" << endl;
        err_page(mfd);
    }
}

int setnonblocking(int sockfd)
{
    if (fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFD, 0)|O_NONBLOCK) == -1)
    {
        return -1;                
    }
    return 0;                
}

int sum_all = 0;
void http_respond_head(int fd,char *type,int len)
{
    char buf[1024];
    sprintf(buf,"HTTP/1.1 200  ok\r\n");
    write(fd,buf,strlen(buf));

    memset(buf,0,sizeof(buf));
    
    sprintf(buf,"Content-Type:%s\r\n",type);
    write(fd,buf,strlen(buf));
    
    sprintf(buf,"Content-Length: %d\r\n",len);
    write(fd,buf,strlen(buf));

    write(fd,"\r\n",2);
}

int main()
{
    pid_t pid;
    pid = fork();
    if(pid > 0)
    {
        exit(0);
    }
    setsid();
    my_thread t1;
    if(chdir("/phpstudy/www") < 0)
    {
        perror("chdir err:");
        return -1;
    }
    int fd = socket(AF_INET,SOCK_STREAM,0);
    if(fd < 0)
    {
        perror("socket err:");
        return -1;
    }
    setnonblocking(fd);
    int yes = 1;
    if(setsockopt(fd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(yes)) < 0)
    {
        perror("setsockopt err:");
        return -1;
    }

    struct sockaddr_in bin_addr;
    bin_addr.sin_family = AF_INET;
    bin_addr.sin_port = htons(8080);
    bin_addr.sin_addr.s_addr = inet_addr("0.0.0.0");
	
    if(bind(fd,(struct sockaddr*)&bin_addr,sizeof(bin_addr)) < 0)
    {
        perror("bind err:");
        return -1;
    }

    if(listen(fd,20) < 0)
    {
        perror("listen err:");
        return -1;
    }
	
	int epofd = epoll_create(2048);
    struct epoll_event ev;
    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = fd;//socket
    if (epoll_ctl(epofd, EPOLL_CTL_ADD,fd, &ev) < 0)
    {
        perror("epoll err:");
        return -1;
    }
    
    struct epoll_event events[2048];
    cout << "进入epoll循环" << endl;
	while(1)
	{
		int ret = epoll_wait(epofd,events,2048,-1);
        if(ret == -1)
        {
            perror("epoll err:");
            return -1;
        }
        for(int i = 0;i < ret;++i)
        {
            cout << "有事件发生" << i << endl;
            struct epoll_event *pev = &events[i];
            if(!(pev->events & EPOLLIN))
            {
                //如果不是读事件
                continue;
            }
            if(pev->data.fd == fd)
            {
                cout << "有新连接" << endl;
            	struct sockaddr_in addr;
            	socklen_t len = sizeof(addr);
            	int new_fd = accept(fd,(struct sockaddr *)&addr,&len);
                //接受连接
                if(new_fd < 0)
                {
                	perror("acc err:");
                	return -1;
				}
				setnonblocking(new_fd);
				struct epoll_event ev;
				ev.events = EPOLLIN | EPOLLET;
                ev.data.fd = new_fd;
                if (epoll_ctl(epofd, EPOLL_CTL_ADD, new_fd, &ev) < 0)
                {
                    cout << "err" << endl;
                    return -1;
                }
            }
            else
            {
                t1.thread_add(p,(void *)pev->data.fd);
                cout << "连接线程成功"  << endl;
            }
        }
    }
}
