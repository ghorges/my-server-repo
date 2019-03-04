#include <stdio.h>

#define MAX 4

int sum = MAX;

void swaps(char *c,char *d)
{
    char t = *c;
    *c = *d;
    *d = t;
}
void digui(char *c,char *d,int len)
{
    if(len == MAX)
    {
        printf("%s\n",d);
    }
    for(int i = len;i < sum;i++)
    {
        swaps(&c[len],&c[i]);
        d[len] = c[len];
        digui(c,d,len + 1);
        swaps(&c[len],&c[i]);

    }
}

int main()
{
    char c[6] = "abcde";
    char d[6] = { 0 };
    digui(c,d,0);
    printf("Hello world\n");
    return 0;
}

