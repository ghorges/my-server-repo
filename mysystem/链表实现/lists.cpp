#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>                                                                                                                                
#include <termios.h>

#define CLEAR()     printf("\ec")     //清屏                                                                                                       
#define CLOSE()     printf("\e[0m")   //关闭所有属性

typedef struct p_poets
{
    char poetName[20];      //诗歌名称
    char creatTime[20];     //创作时间
    char poetMan[20];       //诗人名字
    char poet[500];         //诗歌
    char command[500];      //解释
    struct p_poets *next;         //下一个指针
}POET,*poet;

poet mal()  //malloc放这简单
{
    poet p = (poet)malloc(sizeof(POET));
    if(p == NULL)
    {   
        printf("malloc err\n");
        exit(0);
    }   
    bzero(p,sizeof(p));
    return p;
}


poet input_info()
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

poet end_list(poet phead)
{
    poet p1 = phead;
    while(p1->next != NULL)
    {
        p1 = p1->next;
    }
    return p1;
}

int add_list(poet head)
{
    poet t = input_info();
    poet end_l = end_list(head);
    end_l -> next = t;
    return 0;
}


int remove_list(poet t)
{    
    printf("请输入你想要删除的诗歌名:\n");
    char c[100] = { 0 };
    bzero(c,sizeof(c));
    scanf("%s",c);
    
    while(t->next != NULL)
    {
        if(strcmp(t->next->poetName,c) == 0)
        {
            t->next = t->next->next;
            free(t->next);
            printf("删除成功\n");
            return true;
        }
        t = t->next;
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

    while(t->next != NULL)
    {
        if(strcmp(t->next->poetName,c) == 0)
        {
            poet m = input_info();
            m->next = t->next->next;
            t->next = m;
            free(t->next);   
            printf("改变成功~\n");
            return true;
        }
        t = t->next;
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
    
    t = t->next;
    while(t != NULL)
    {
        if(strcmp(t->poetName,c) == 0)
        {
            return t;
        }
        t = t->next;
    }
    return NULL;
}


void show_list(poet t)
{
    //因为t是头结点,头节点没值
    t = t->next;
    int i = 0;
    while(t != NULL)
    {
        printf("-----------------第%d首诗歌---------------\n",++i);
        printf("诗歌名称:%s\n",t->poetName);
        printf("创作时间:%s\n",t->creatTime);
        printf("诗人:%s\n",t->poetMan);
        printf("诗歌内容:%s\n",t->poet);
        printf("诗歌注释:%s\n",t->command);
        //指向下一个节点
        t = t->next;
    }
}


int sort_list(poet head)
{
    poet aaa = head->next;    
    poet anext = head;
    int i = 0;
    for(;aaa->next != NULL;aaa = aaa->next,anext = anext->next)
    {
        for(poet bbb = aaa->next;bbb->next != NULL;bbb = bbb->next)
        {
            if(strcmp(aaa->poetName,bbb->poetName) < 0)
            {
                aaa->next = bbb->next;
                bbb->next = aaa;
                anext->next = bbb;
            }
        }
    }
}
