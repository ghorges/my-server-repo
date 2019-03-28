#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <ostream>

using namespace std;
class my_string
{
    friend ostream &operator<<(ostream &p,my_string &q);
    public:
    	typedef char* iterator;
        //赋值构造
        my_string(char *s = NULL)
        {
            if(s == NULL)
            {
                this->s = NULL;
                return;
            }
            this->s = (char *)calloc(strlen(s),1);
            strcpy(this->s,s);
            
        }
        my_string()
        {
            this->s = NULL;
        }
        //拷贝构造
        my_string(const my_string &p)
        {
            if(p.s == NULL)
            {
                this->s = NULL;
            }
            else
            {
                this->s = (char *)calloc(strlen(p.s),1);
                strcpy(this->s,p.s);
            }
            cout << "拷贝构造成功" << endl;
        }
        //析构
        ~my_string()
        {
            if(s == NULL)
            {
                
            }
            else
            {
                free(s);
            }
        }
        //得到长度
        int length()
        {
            return strlen(this->s);
        }
        //insert,在什么地方insert
        int insert(int a,char *p)
        {
            int len = strlen(this->s);
            this->s = (char *)realloc(this->s,len + strlen(p));
            if(this->s == NULL)
            {   
                cout << "分配内存失败" << endl;
                return -1; 
            }
            if(a >= len)
            {
                strcat(s,p);
            }
            else
            {
                int it_len = strlen(&s[a]);
                char *q = (char *)calloc(it_len,1);
                strcpy(q,&s[a]);
                strcpy(&s[a],p);
                strcpy(&s[a + strlen(p)],q);
                free(q);
            }
        }
        void operator=(my_string &p)
        {
            if(s == NULL)
            {
                if(p.s == NULL)
                {
                    return;
                }
                else
                {
                    this->s = (char *)calloc(strlen(p.s),1);
                    strcpy(this->s,p.s);
                    return;
                }
            }
            else
            {
                free(this->s);
                s = NULL;
                if(p.s == NULL)
                {
                    return;
                }
                else
                {
                    this->s = (char *)calloc(strlen(p.s),1);
                    strcpy(this->s,p.s);
                    return;
                }

            }
        }
        //删除字串,第一个是从多少开始，第二个是删除多少个
        char* erase(int a,int b)
        {
            if(a >= strlen(this->s))
            {
                return 0;
            }
            return s + a;
        }
        //+=
        my_string & operator+=(my_string &p)
        {
            this->s = (char *)realloc(this->s,strlen(this->s) + strlen(p.s));
            strcat(this->s,p.s);
            return *this;
        }
        // []
        char &operator[](int a)
        {
            return s[a];
        }
        //!=
        int operator!=(my_string &p)
        {
            return strcmp(this->s,p.s);
        }
        //==
        int operator==(my_string &p)
        {
            return !strcmp(this->s,p.s);
        }
        
        char* begin()
        {
        	return s;	
		}
		
		char* end()
		{
			return (s+strlen(s));
		}
    private:
        char *s;
};

ostream &operator<<(ostream &p,my_string &q)
{
    cout << q.s;
    return p;
}

int main()
{
    //my_string c("123");
    //my_string d(c);
    //cout << d << endl;
    //my_string c = "132";
    //c.insert(2,"1111");
    //cout << c << endl;

    //my_string c("132");
    //my_string d("222");
    //c += d;
    //cout << c[1] << endl;
    //c[1] = '1';
    //cout << c << endl;
    my_string c("123");
    my_string d("123");
    for(my_string::iterator t = c.begin();t != c.end();t++)
    {
    	cout << *t;
	}
    int a = (c == d);
    cout << a << endl;
    return 0;
}
