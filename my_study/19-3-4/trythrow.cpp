#include <iostream>

using namespace std;

void divide(int a,int b)
{
    if(b == 0)
    {
        //相当于return，后面的子句不能被执行
        throw(b);
    }
    cout << "a / b = " << a/b << endl;
}

int main()
{
    try
    {
        divide(10,2);
        divide(100,0);
    }
    catch(int e)
    {
        cout << e << "被0除" << endl;
    }
    //...表示未知数据类型的异常
    catch(...)
    {
        cout << "其他未知类型异常" << endl;
    }
    std::cout << "Hello world" << std::endl;
    return 0;
}

