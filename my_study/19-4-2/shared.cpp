#include <iostream>
#include <memory>
#include <string>

using namespace std;

int main()
{
    /*
    int a = 10;
    shared_ptr<int>t = make_shared<int>(a);
    //这个t是一个指针
    
    //cout << t << endl;
    cout << *t << endl;
    
    *t = 200;
    cout << *t << endl;
    cout << a << endl;
    */

    shared_ptr<string>p = make_shared<string>(10,'9');
    cout << *p << endl;
    std::cout << "Hello world" << std::endl;
    return 0;
}
