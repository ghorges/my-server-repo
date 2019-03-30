#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>                                                                                                                                
#include <termios.h>
//#include "lists.cpp"
//注册

#define CLEAR()     printf("\ec")     //清屏   
#define CLOSE()     printf("\e[0m")   //关闭所有属性




typedef struct
{
    unsigned int count[2];
    unsigned int state[4];
    unsigned char buffer[64];   
}MD5_CTX;
 
                         
#define F(x,y,z) ((x & y) | (~x & z))
#define G(x,y,z) ((x & z) | (y & ~z))
#define H(x,y,z) (x^y^z)
#define I(x,y,z) (y ^ (x | ~z))
#define ROTATE_LEFT(x,n) ((x << n) | (x >> (32-n)))
#define FF(a,b,c,d,x,s,ac) \
          { \
          a += F(b,c,d) + x + ac; \
          a = ROTATE_LEFT(a,s); \
          a += b; \
          }
#define GG(a,b,c,d,x,s,ac) \
          { \
          a += G(b,c,d) + x + ac; \
          a = ROTATE_LEFT(a,s); \
          a += b; \
          }
#define HH(a,b,c,d,x,s,ac) \
          { \
          a += H(b,c,d) + x + ac; \
          a = ROTATE_LEFT(a,s); \
          a += b; \
          }
#define II(a,b,c,d,x,s,ac) \
          { \
          a += I(b,c,d) + x + ac; \
          a = ROTATE_LEFT(a,s); \
          a += b; \
          }                                            
void MD5Init(MD5_CTX *context);
void MD5Update(MD5_CTX *context,unsigned char *input,unsigned int inputlen);
void MD5Final(MD5_CTX *context,unsigned char digest[16]);
void MD5Transform(unsigned int state[4],unsigned char block[64]);
void MD5Encode(unsigned char *output,unsigned int *input,unsigned int len);
void MD5Decode(unsigned int *output,unsigned char *input,unsigned int len);


unsigned char PADDING[]={0x80,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                         0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                         0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                         0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
                         
void MD5Init(MD5_CTX *context)
{
     context->count[0] = 0;
     context->count[1] = 0;
     context->state[0] = 0x67452301;
     context->state[1] = 0xEFCDAB89;
     context->state[2] = 0x98BADCFE;
     context->state[3] = 0x10325476;
}
void MD5Update(MD5_CTX *context,unsigned char *input,unsigned int inputlen)
{
    unsigned int i = 0,index = 0,partlen = 0;
    index = (context->count[0] >> 3) & 0x3F;
    partlen = 64 - index;
    context->count[0] += inputlen << 3;
    if(context->count[0] < (inputlen << 3))
       context->count[1]++;
    context->count[1] += inputlen >> 29;
    
    if(inputlen >= partlen)
    {
       memcpy(&context->buffer[index],input,partlen);
       MD5Transform(context->state,context->buffer);
       for(i = partlen;i+64 <= inputlen;i+=64)
           MD5Transform(context->state,&input[i]);
       index = 0;        
    }  
    else
    {
        i = 0;
    }
    memcpy(&context->buffer[index],&input[i],inputlen-i);
}
void MD5Final(MD5_CTX *context,unsigned char digest[16])
{
    unsigned int index = 0,padlen = 0;
    unsigned char bits[8];
    index = (context->count[0] >> 3) & 0x3F;
    padlen = (index < 56)?(56-index):(120-index);
    MD5Encode(bits,context->count,8);
    MD5Update(context,PADDING,padlen);
    MD5Update(context,bits,8);
    MD5Encode(digest,context->state,16);
}
void MD5Encode(unsigned char *output,unsigned int *input,unsigned int len)
{
    unsigned int i = 0,j = 0;
    while(j < len)
    {
         output[j] = input[i] & 0xFF;  
         output[j+1] = (input[i] >> 8) & 0xFF;
         output[j+2] = (input[i] >> 16) & 0xFF;
         output[j+3] = (input[i] >> 24) & 0xFF;
         i++;
         j+=4;
    }
}
void MD5Decode(unsigned int *output,unsigned char *input,unsigned int len)
{
     unsigned int i = 0,j = 0;
     while(j < len)
     {
           output[i] = (input[j]) |
                       (input[j+1] << 8) |
                       (input[j+2] << 16) |
                       (input[j+3] << 24);
           i++;
           j+=4; 
     }
}
void MD5Transform(unsigned int state[4],unsigned char block[64])
{
     unsigned int a = state[0];
     unsigned int b = state[1];
     unsigned int c = state[2];
     unsigned int d = state[3];
     unsigned int x[64];
     MD5Decode(x,block,64);
     FF(a, b, c, d, x[ 0], 7, 0xd76aa478); /* 1 */
 FF(d, a, b, c, x[ 1], 12, 0xe8c7b756); /* 2 */
 FF(c, d, a, b, x[ 2], 17, 0x242070db); /* 3 */
 FF(b, c, d, a, x[ 3], 22, 0xc1bdceee); /* 4 */
 FF(a, b, c, d, x[ 4], 7, 0xf57c0faf); /* 5 */
 FF(d, a, b, c, x[ 5], 12, 0x4787c62a); /* 6 */
 FF(c, d, a, b, x[ 6], 17, 0xa8304613); /* 7 */
 FF(b, c, d, a, x[ 7], 22, 0xfd469501); /* 8 */
 FF(a, b, c, d, x[ 8], 7, 0x698098d8); /* 9 */
 FF(d, a, b, c, x[ 9], 12, 0x8b44f7af); /* 10 */
 FF(c, d, a, b, x[10], 17, 0xffff5bb1); /* 11 */
 FF(b, c, d, a, x[11], 22, 0x895cd7be); /* 12 */
 FF(a, b, c, d, x[12], 7, 0x6b901122); /* 13 */
 FF(d, a, b, c, x[13], 12, 0xfd987193); /* 14 */
 FF(c, d, a, b, x[14], 17, 0xa679438e); /* 15 */
 FF(b, c, d, a, x[15], 22, 0x49b40821); /* 16 */
 
 /* Round 2 */
 GG(a, b, c, d, x[ 1], 5, 0xf61e2562); /* 17 */
 GG(d, a, b, c, x[ 6], 9, 0xc040b340); /* 18 */
 GG(c, d, a, b, x[11], 14, 0x265e5a51); /* 19 */
 GG(b, c, d, a, x[ 0], 20, 0xe9b6c7aa); /* 20 */
 GG(a, b, c, d, x[ 5], 5, 0xd62f105d); /* 21 */
 GG(d, a, b, c, x[10], 9,  0x2441453); /* 22 */
 GG(c, d, a, b, x[15], 14, 0xd8a1e681); /* 23 */
 GG(b, c, d, a, x[ 4], 20, 0xe7d3fbc8); /* 24 */
 GG(a, b, c, d, x[ 9], 5, 0x21e1cde6); /* 25 */
 GG(d, a, b, c, x[14], 9, 0xc33707d6); /* 26 */
 GG(c, d, a, b, x[ 3], 14, 0xf4d50d87); /* 27 */
 GG(b, c, d, a, x[ 8], 20, 0x455a14ed); /* 28 */
 GG(a, b, c, d, x[13], 5, 0xa9e3e905); /* 29 */
 GG(d, a, b, c, x[ 2], 9, 0xfcefa3f8); /* 30 */
 GG(c, d, a, b, x[ 7], 14, 0x676f02d9); /* 31 */
 GG(b, c, d, a, x[12], 20, 0x8d2a4c8a); /* 32 */
 
 /* Round 3 */
 HH(a, b, c, d, x[ 5], 4, 0xfffa3942); /* 33 */
 HH(d, a, b, c, x[ 8], 11, 0x8771f681); /* 34 */
 HH(c, d, a, b, x[11], 16, 0x6d9d6122); /* 35 */
 HH(b, c, d, a, x[14], 23, 0xfde5380c); /* 36 */
 HH(a, b, c, d, x[ 1], 4, 0xa4beea44); /* 37 */
 HH(d, a, b, c, x[ 4], 11, 0x4bdecfa9); /* 38 */
 HH(c, d, a, b, x[ 7], 16, 0xf6bb4b60); /* 39 */
 HH(b, c, d, a, x[10], 23, 0xbebfbc70); /* 40 */
 HH(a, b, c, d, x[13], 4, 0x289b7ec6); /* 41 */
 HH(d, a, b, c, x[ 0], 11, 0xeaa127fa); /* 42 */
 HH(c, d, a, b, x[ 3], 16, 0xd4ef3085); /* 43 */
 HH(b, c, d, a, x[ 6], 23,  0x4881d05); /* 44 */
 HH(a, b, c, d, x[ 9], 4, 0xd9d4d039); /* 45 */
 HH(d, a, b, c, x[12], 11, 0xe6db99e5); /* 46 */
 HH(c, d, a, b, x[15], 16, 0x1fa27cf8); /* 47 */
 HH(b, c, d, a, x[ 2], 23, 0xc4ac5665); /* 48 */
 
 /* Round 4 */
 II(a, b, c, d, x[ 0], 6, 0xf4292244); /* 49 */
 II(d, a, b, c, x[ 7], 10, 0x432aff97); /* 50 */
 II(c, d, a, b, x[14], 15, 0xab9423a7); /* 51 */
 II(b, c, d, a, x[ 5], 21, 0xfc93a039); /* 52 */
 II(a, b, c, d, x[12], 6, 0x655b59c3); /* 53 */
 II(d, a, b, c, x[ 3], 10, 0x8f0ccc92); /* 54 */
 II(c, d, a, b, x[10], 15, 0xffeff47d); /* 55 */
 II(b, c, d, a, x[ 1], 21, 0x85845dd1); /* 56 */
 II(a, b, c, d, x[ 8], 6, 0x6fa87e4f); /* 57 */
 II(d, a, b, c, x[15], 10, 0xfe2ce6e0); /* 58 */
 II(c, d, a, b, x[ 6], 15, 0xa3014314); /* 59 */
 II(b, c, d, a, x[13], 21, 0x4e0811a1); /* 60 */
 II(a, b, c, d, x[ 4], 6, 0xf7537e82); /* 61 */
 II(d, a, b, c, x[11], 10, 0xbd3af235); /* 62 */
 II(c, d, a, b, x[ 2], 15, 0x2ad7d2bb); /* 63 */
 II(b, c, d, a, x[ 9], 21, 0xeb86d391); /* 64 */
     state[0] += a;
     state[1] += b;
     state[2] += c;
     state[3] += d;
}






void menu(poet head);
int login();
void regist();
void users_login(poet head);

int getch()
{
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    return ch;
}


void regist()
{
    CLEAR();
   FILE *fp=fopen("users.txt","r+");/*以写模式("w")打开文件user.txt,如果不存在,会自动创建*/
   char name[1000] = {0};
loop:
   printf("请输入您要注册的账号:");
   scanf("%s",name);

   printf("请输入您的密码:");
   char pas[1000] = {0};
   scanf("%s",pas); /*输入名称和密码*/

   //printf("请妥善保管你的账号和密码~\n账号:%s\n",name);
   //printf("密码:%s\n",pas);

   char a[80]={0};
   char b[80]={0};
   while(!feof(fp))
   {
   fgets(a,80,fp);
   for(int i=0;i<strlen(a);i++)
   {
      if(a[i]==' ')
      {
          break;
      }
      b[i]=a[i];
   }
   if(strcmp(name,b)==0)
   {
        printf("--------------------------您的账号已被注册!!!-----------------\n");
        printf("-----------------------请重新输入你的账号和密码---------------\n");
        sleep(1);
        goto loop;
   }
   }
    printf("请妥善保管你的账号和密码~\n账号:%s\n",name);
    printf("密码:%s\n",pas);
    sleep(1);
    char c1111 = 0;
	unsigned char decrypt[16];
    char c221 = 0;
	MD5_CTX md5;
	MD5Init(&md5);         		
	MD5Update(&md5,(unsigned char*)pas,strlen(pas));
	MD5Final(&md5,decrypt);  
    fputs(name,fp);
    fputs(" ",fp);
    
    
    printf("%s\n",decrypt);
    fwrite(decrypt,16,1,fp);
    //fputs((char *)decrypt,fp);/*将名称和密码以字符串形式写入文件*/
    //sleep(10);
   fputs("\n",fp);
   fflush(stdin);//清除缓存
   fclose(fp);/*关闭文件*/
    return;
}

//登录
int login()
{
    printf("登录界面\n");
    char name[200];
    char pass[200];
    int ok = 0;
    printf("请输入账号:");
    scanf("%s",name);
    printf("请输入密码:");
    scanf("%s",pass); /*输入名称和密码*/
    strcat(name," ");

    char aaaa = 0;
	unsigned char decrypt[16]; 
    char bbbbbb = 0;
	MD5_CTX md5;
	MD5Init(&md5);         		
	MD5Update(&md5,(unsigned char*)pass,strlen(pass));
	MD5Final(&md5,decrypt); 

    strncat(name,(const char*)decrypt,16);//合并字符
    strcat(name,"\n");//添加换行字符
    int len = 0;
    FILE *fp = fopen("users.txt", "r");/*允许读写文件*/
    if(NULL == fp)
    {
        printf("无法打开文件\n");
        return false;
    }

    while(!feof(fp))
    {
        char textstring[100] = {0};
        memset(textstring, 0, sizeof(textstring));//清空
        fgets(textstring, sizeof(textstring), fp); // 从文件获取字符串函数包含了\n
        if(strcmp(name,textstring)==0)
        {
            ok=1;
            break;
        }
    }
    if(ok){
      printf("登录OK\n");
      sleep(1);
      printf("\033c");
      return true;
    }
    else{
      printf("登录失败\n");
      fclose(fp);
      return false;
    }
}

void users_login(poet head)
{
    while(1)
    {
        CLEAR();
    printf("----------\n");
    printf("1.用户注册\n");
    printf("2.用户登录\n");
    printf("----------\n");
    printf("请输入您的选择:");
    char t;
    t=getch();
    if(t == '1')
    {
        regist();
        continue;
    }
    else if(t == '2')
    {
        int r = login();
        if(r == true)
        menu(head);
        else
        {
            printf("您的用户不存在或密码错误,请重新输入(按任意键继续)\n");
            getchar();
            getchar();
        }
    }
    else
    {
        printf("您的输入有误,请重新输入\n");
        usleep(500);
    }
    
    }
}

void logout(poet head)
{
    FILE *fp=fopen("output.txt","w");
    poet t = head->next;
    while(t)
    {
        fprintf(fp,"%s\n",t->poetName);
        fprintf(fp,"%s\n",t->creatTime);
        fprintf(fp,"%s\n",t->poetMan);
        fprintf(fp,"%s\n",t->poet);
        fprintf(fp,"%s\n",t->command);
        t = t->next;
    }
    fclose(fp);
    exit(0);
}

void a_sys(poet head)
{
    poet t =  end_list(head);
    poet inputs = input_info();
    t->next = inputs;
    
    printf("-------按任意键继续------\n");
    getchar();
    getchar();
}
void b_sys(poet head)
{
    change_list(head);                                                                                                                              

    printf("-------按任意键继续------\n");
    getchar();
    getchar();
}
void c_sys(poet head)
{
     remove_list(head);                                                                                                                                              

    printf("-------按任意键继续------\n");
    getchar();
    getchar();
}
void d_sys(poet head)
{
    poet t = NULL;
    t = find_list(head);                                                                                                                                              
    if(t == NULL)
    {
        printf("---------------诗歌不存在-------------------\n");
    }
    else
    {
        printf("-------------------------------------------------\n");
        printf("|                  诗歌名称:%s                   |\n",t->poetName);
        printf("|                  创作时间:%s                   |\n",t->creatTime);
        printf("|                  诗人:%s                       |\n",t->poetMan);
        printf("|                  诗歌内容:%s                   |\n",t->poet);
        printf("|                  诗歌注释:%s                   |\n",t->command);
        printf("-------------------------------------------------\n");
    }

    printf("-------按任意键继续------\n");
    getchar();
    getchar();
}
void e_sys(poet head)
{
    printf("-------按任意键继续------\n");
    getchar();                                                                                                                          
}
void f_sys(poet head)
{
    sort_list(head);
    printf("排序成功~\n");

    show_list(head);
    printf("-------按任意键继续------\n");
    getchar();
                                                                                                                                                   
}


void g_sys(poet head)
{
    show_list(head);
    printf("-------按任意键继续------\n");
    getchar();

}


void menu(poet head)
{
    do
    {

        CLEAR();

        printf("------------------------诗歌信息管理系统------------------\n\n");
        printf("|                         a.数据录入.                    | \n");
        printf("|                         b.数据修改.                    | \n");
        printf("|                         c.数据删除.                    |\n");
        printf("|                         d.数据查询.                    |\n");
            //printf("  e.数据统计.\n");
        printf("|                         f.数据排序.                    |\n");
        printf("|                         g.打印报表.                    |\n");
        printf("|                         h.退出系统.                    |\n");
        printf("---------------------------------------------------------\n")
    char t;
    t=getch();
    //printf("cout: %c\n",t);
    switch(t)
    {
        case 'a': a_sys(head);break;        //数据录入
        case 'b': b_sys(head);break;       //  修改
        case 'c': c_sys(head);break;       //  删除
        case 'd': d_sys(head);break;         // 查找
        //case 'e': e_sys(head);break;     //统计
        case 'f': f_sys(head);break;      //排序
        case 'g': g_sys(head);break;      //  打印
        case 'h': logout(head);break;
        default:  break;
    }
    }while(1);

}
