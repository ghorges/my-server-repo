#include <iostream>
#include <stdexcept>
//自己重新写exception函数，和php一样，语言是共通的
using namespace std;

class my_except : public exception
{
public:
    my_except(const char * t)
    {
        this->my_char = const_cast<char *>(t);
    }

    virtual const char * what()
    {
        cout << "my_except:" << my_char << endl;
        return my_char;
    }
private:
    char *my_char;
};
void aaa()
{
    throw my_except("我自己的错误");
}
int main()
{
    try
    {
        aaa();
    }
    catch(my_except &t)
    {
        cout << "捕获成功" << endl;
        cout << t.what() << endl;
    }
    catch(...)
    {
        cout << "其他位置类型错误" << endl;
    }
    std::cout << "Hello world" << std::endl;
    return 0;
}

