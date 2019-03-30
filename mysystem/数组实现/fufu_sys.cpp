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

POET head[100];
void read()
{
    FILE *fp;
    fp=fopen("output.txt","r");
        fscanf(fp,"%s",head[n].poetName);
        fscanf(fp,"%s",head[n].creatTime);
        fscanf(fp,"%s",head[n].poetMan);
        fscanf(fp,"%s",head[n].poet);
        fscanf(fp,"%s",head[n].command);
        if(strlen(head[n].poetName) > 0)
        n++;
    while(!feof(fp))
    {
        fscanf(fp,"%s",head[n].poetName);
        fscanf(fp,"%s",head[n].creatTime);
        fscanf(fp,"%s",head[n].poetMan);
        fscanf(fp,"%s",head[n].poet);
        fscanf(fp,"%s",head[n].command);
        n++;
    }
    fclose(fp);
}

int main()
{
    //head->next = input_info();
    read();
    show_list(head);
    
    users_login(head);
    std::cout << "Hello world" << std::endl;
    return 0;
}
