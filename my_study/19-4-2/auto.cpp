#include <iostream>

using namespace std;
//g++ -std=c++11 auto.cpp

int mm()
{
    cout << "111" << endl;
}
int main()
{
    auto i = 12;
    cout << i << endl;

    auto j = 11,*p = &j;
    cout << *p << endl;
    
    const int &h = 42;
    //auto &h = 42;
    cout << h << endl;
    
    decltype(i) t = 13;
    cout << t << endl;
   
    decltype((i)) t1 = i;
    cout << t1 << endl;
    
    decltype(mm()) aaa = 333;
    cout << aaa << endl;
    return 0;

}

