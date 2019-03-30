#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>                                                                                                                                
#include <termios.h>

#define CLEAR()     printf("\ec")     //清屏                                                                                                       
#define CLOSE()     printf("\e[0m")   //关闭所有属性

int n;

typedef struct p_poets
{
    char poetName[20];      //诗歌名称
    char creatTime[20];     //创作时间
    char poetMan[20];       //诗人名字
    char poet[500];         //诗歌
    char command[500];      //解释
    struct p_poets *next;         //下一个指针
}POET,*poet;



/*poet input_info()
{
    poet t = mal();
    printf("请输入诗歌名称:");
    scanf("%s",t->poetName);
    printf("请输入创作时间:");
    scanf("%s",t->creatTime);
    printf("请输入诗人名字:");
    scanf("%s",t->poetMan);
    printf("请输入诗歌:");
    scanf("%s",t->poet);
    printf("请输入注释:");
    scanf("%s",t->command);
    printf("录入完成~\n");
    return t;
}
*/


int add_list(poet head)
{
    POET t;                                                      
    printf("请输入诗歌名称:");
    scanf("%s",t.poetName);
    printf("请输入创作时间:");
    scanf("%s",t.creatTime);
    printf("请输入诗人名字:");
    scanf("%s",t.poetMan);
    printf("请输入诗歌:");
    scanf("%s",t.poet);
    printf("请输入注释:");
    scanf("%s",t.command);
    head[n++] = t;
    printf("录入完成~\n");

    return 0;
}


int remove_list(poet t)
{    
    printf("请输入你想要删除的诗歌名:\n");
    char c[100] = { 0 };
    bzero(c,sizeof(c));
    scanf("%s",c);
    
    for(int i = 0;i < n;i++)
    {
        if(strcmp(t[i].poetName,c) == 0)
        {
            for(int j = i;j < n;j++)
            {
                t[i] = t[i + 1];
            }
            n--;
            printf("删除成功\n");
            return true;
        }
    }

    printf("删除失败\n");
    return false;
}


int change_list(poet t)
{
    printf("请输入你想要改变的诗歌的诗歌名:\n");
    char c[100] = { 0 };
    bzero(c,sizeof(c));
    scanf("%s",c);

    for(int i = 0;i < n;i++)
    {
        if(strcmp(t[i].poetName,c) == 0)
        {
                POET mm;                                                      
    printf("请输入诗歌名称:");
    scanf("%s",mm.poetName);
    printf("请输入创作时间:");
    scanf("%s",mm.creatTime);
    printf("请输入诗人名字:");
    scanf("%s",mm.poetMan);
    printf("请输入诗歌:");
    scanf("%s",mm.poet);
    printf("请输入注释:");
    scanf("%s",mm.command);
    t[i] = mm;
    printf("改变完成~\n");
            return true;
        }
    }

    printf("改变失败,请确认此诗歌是否存在!\n");
    return false;  
}

//find,成功返回地址,失败返回空
poet find_list(poet t)
{
    printf("请输入你想要查询的诗歌名:\n");
    char c[100] = { 0 };
    bzero(c,sizeof(c));
    scanf("%s",c);
    
    for(int i = 0;i < n;i++)
    {
        if(strcmp(t[i].poetName,c) == 0)
        {
            return &t[i];
        }
    }
    return NULL;
}


void show_list(poet head)
{
    //因为t是头结点,头节点没值
    int i = 0;
    for(int j = 0;j < n;j++)
    {
        printf("-----------------第%d首诗歌---------------\n",++i);
        printf("诗歌名称:%s\n",head[j].poetName);
        printf("创作时间:%s\n",head[j].creatTime);
        printf("诗人:%s\n",head[j].poetMan);
        printf("诗歌内容:%s\n",head[j].poet);
        printf("诗歌注释:%s\n",head[j].command);
        //指向下一个节点
    }
}


int sort_list(poet head)
{
    int i = 0;
    for(int i = 0;i < n - 1;i++)
    {
        for(int j = 1;j < n;j++)
        {
            if(strcmp(head[i].poetName,head[j].poetName) > 0)
            {
                POET t = head[i];
                head[i] = head[j];
                head[j] = t;
            }
        }
    }
}
