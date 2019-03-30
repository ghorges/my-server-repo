#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>                                                                                                                                
#include <termios.h>
#include <iostream>
#include "lists.cpp"
#include "userlogin.cpp"
using namespace std;

#define CLEAR()     printf("\ec")     //清屏   
#define CLOSE()     printf("\e[0m")   //关闭所有属性


poet read(const poet phead)
{
    FILE *fp;
    fp=fopen("output.txt","r");
    poet pnew,pend;
    pend=phead;

    pnew=mal();
        fscanf(fp,"%s",pnew->poetName);
        fscanf(fp,"%s",pnew->creatTime);
        fscanf(fp,"%s",pnew->poetMan);
        fscanf(fp,"%s",pnew->poet);
        fscanf(fp,"%s",pnew->command);
        
    while(!feof(fp))
    {
        pend->next=pnew;
        pend=pnew;
        pnew=(poet)malloc(sizeof(POET));
        fscanf(fp,"%s",pnew->poetName);
        fscanf(fp,"%s",pnew->creatTime);
        fscanf(fp,"%s",pnew->poetMan);
        fscanf(fp,"%s",pnew->poet);
        fscanf(fp,"%s",pnew->command);
    }
    fclose(fp);
    return phead;
}

int main()
{
    poet head = mal();
    //head->next = input_info();
    read(head);
    show_list(head);
    
    users_login(head);
    /*
    add_list(head);
    show_list(head);

    if(find_list(head) != NULL)
    {
        cout << "查找成功" << endl;
    }
    else
    {
        cout << "查找失败" << endl;
    }

    remove_list(head);

    change_list(head);
    show_list(head);
    */
    std::cout << "Hello world" << std::endl;
    return 0;
}
