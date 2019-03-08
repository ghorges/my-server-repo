#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main()
{
    vector <int> t;
    t.push_back(1);
    t.push_back(3);
    t.push_back(5);

    for(vector <int>::iterator it = t.begin();it != t.end();it++)
    {
        cout << *it << endl;
    }

    int num1 = count(t.begin(),t.end(),3);
    cout << "num1 = " << num1 << endl;
    std::cout << "Hello world" << std::endl;
    return 0;
}

