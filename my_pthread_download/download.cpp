#include <iostream>                                                                                                                                
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>

using namespace std;
char dns_ip[100];


struct hostent *host;
int l_length = 0;

char *argvss11;
//获取主机地址dns解析
void get_host(char *hostName)
{
    char ip[100];
    struct hostent *host;
    struct in_addr **addr_list;
    int i;

    if (NULL == (host = gethostbyname(hostName))) {
        perror("get host by name error");
        exit(1);
    }

    addr_list = (struct in_addr **)host->h_addr_list;
    for (i = 0; addr_list[i] != NULL; i++) {
        // inet_ntoa()将long类型的IP地址转化为圆点的字符串形式，作用与inet_addr()相反
        strcpy(dns_ip, inet_ntoa(*addr_list[i]));
    }

    printf("%s resolved to: %s", hostName, dns_ip);
}

void get_mes(int fd)
{
    lseek(fd,SEEK_SET,0);
    char c[1000];
    bzero(c,sizeof(c));
    int t = read(fd,c,1000);
    if(t < 0)
    {
        if(t < 0)
        {
            perror("write err:");
            exit(0);
        }                       

    }
    //cout << c << endl;
    int flag_len = 0;
    int flag_by = 0;
    for(int i = 0;i != strlen(c);i++)
    {
        if(c[i] == 'A' && c[i + 1] == 'c' && c[i + 2] == 'c' && c[i + 3] == 'e' && c[i + 4] == 'p' && c[i + 5] == 't' && c[i + 6] == '-' && c[i + 7] == 'R' && c[i + 8] == 'a' && c[i + 9] == 'n' && c[i + 10] == 'g' && c[i + 11] == 'e' && c[i + 12] == 's')
        {
            flag_by = 1;
            if((c[i + 14] == 'b' && c[i + 15] == 'y') || (c[i + 15] == 'b' && c[i + 16] == 'y'))
            {
                cout << "此链接支持多线程下载" << endl;
            }
            else
            {
                cout << "此链接不支持多线程下载，Sorry～" << endl;
                exit(0);
            }
                
        }
        if(c[i] == 'C' && c[i + 3] == 't' && c[i + 5] == 'n' && c[i + 8] == 'L' && c[i + 9] == 'e')
        {
            flag_len = 1;
            if(c[i + 15] != ' ')
            {
                l_length = atoi(&c[i + 15]);
            }
            else             
            {
                l_length = atoi(&c[i + 16]);
            }
        }
    }
    if(flag_by == 0)
    {
        cout << "此链接不支持多线程下载，Sorry～" << endl;
        exit(0);
    }
    if(flag_len == 0)
    {
        cout << "该文件没有给定长度,Sorry～" << endl;
        exit(0);
    }
}
void *pth_arr(void *p)
{
    int fd = socket(AF_INET,SOCK_STREAM,0);
    if(fd < 0)
    {   
        perror("socket err:");
        exit(0); 
    } 

    struct sockaddr_in addrs;
    bzero(&addrs,sizeof(addrs));

    addrs.sin_family = AF_INET;
    addrs.sin_port = htons(80);  

    addrs.sin_addr.s_addr = inet_addr(dns_ip);
    if(connect(fd, (struct sockaddr *)&addrs,sizeof(addrs)) < 0)
    {   
        perror("conn err:");
        exit(0); 
    }
    
    char get_range[1000];
    bzero(get_range,sizeof(get_range));
    sprintf(get_range,"GET %s HTTP/1.1\r\n",argvss11);
    cout << get_range << endl;
    send(fd,get_range,sizeof(get_range),0);

    bzero(get_range,sizeof(get_range));
    sprintf(get_range,"Range");
    cout << "conn success" << endl;

}
int main(int argc,char *argv[])
{
    if(argc < 3)
    {
        cout << "格式:./a.out [你要下载的地址] [你要启动的线程数]" << endl;
        return -1;
    }
    argvss11 = argv[1];
    int fd = open("www.txt",O_CREAT | O_RDWR,0666);
    int fd2 = dup(STDOUT_FILENO);         
    if(fd < 0)
    {
        perror("open www.txt err:");
        return -1;
    }
    int pid = vfork();
    if(pid < 0)
    {
        perror("fork err:");
        return -1;
    }
    else if(pid == 0)
    {
        int r = dup2(fd,STDOUT_FILENO);
        if(r != STDOUT_FILENO)
        {
            perror("dup2 err:");
            return -1;
        }
        char *c[10];
        char *envp[] = { NULL };
        bzero(c,sizeof(c));
        c[0] = "/usr/bin/curl";
        c[1] = "-I";
        c[2] = "-s";
        c[3] = argv[1];
        execve("/usr/bin/curl",c,envp);
    }
    cout << "正在获取资源，请耐心等待～" << endl;
    wait(NULL);                            
    int r = dup2(fd2,STDOUT_FILENO);
    if(r < 0)
    {
        perror("dup2 err:");
        exit(0);
    }
    try
    {
        get_mes(fd);
        if(l_length < 1024)
        {
            cout << "文件大小为:" << l_length << "bytes" << endl;
        }
        else if(l_length < 1024 * 1024)
        {
            float l_lem = 1.0 * l_length / 1024  * 1.0;
            cout << "文件大小为:" << l_lem << "KB" << endl;
        }
        else
        {
            float l_lem = 1.0 * l_length / 1024 / 1024 * 1.0;
            cout << "文件大小为:" << l_lem << "MB" << endl;
        }
        
        
        int pth = atoi(argv[2]);
        if(pth <= 0)
        {
            cout << "线程不能小于0 err~" << endl;  
        //dns解析
        get_host(argv[1]);


        /*
         * 这样也可以
        char aaaaip[16] = { 0 };
        host = gethostbyname(argv[1]);
        inet_ntop(host->h_addrtype,host->h_addr_list[0],aaaaip,sizeof(aaaaip));
        cout << "````````````````" << aaaaip << endl;
        */
        pth_arr(NULL);
        //这个是分包分的大小
        /*
        int pth_len = l_length / pth;
        for(int i = 0;i <= pth;i++)
        {
            pthread_t tid;
            struct
            {
                int a;
                int b;
            }mmm;

            mmm m1;
            if(i * pth_len == l_length)
                break;
            m1.a = i * pth_len;
            if(m1.b < i * pth_len)
            {                       
                m1.b = i * pth_len;
            }
            else
            {
                m1.b = l_length;
            }
            if(pthread_create(&tid),NULL,pth_arr,);
        }
        */

    }
    
    }
    catch(int t)
    {
        ;
    }
    catch(...)
    {
        cout << "未知类型错误" << endl;
    }
    cout << "hello world" << endl;
    return 0;
}                                    
