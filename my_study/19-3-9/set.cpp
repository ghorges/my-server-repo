#include <iostream>
#include <set>

using namespace std;


int main()
{
    set <int> s1;
    s1.insert(10);
    s1.insert(2);
    s1.insert(100);

    for(set<int>::iterator it = s1.begin();it != s1.end();it++)
    {
        cout << *it << endl;
    }

    while(!s1.empty())
    {
        s1.erase(s1.begin());
    }

    std::cout << "Hello world" << std::endl;
    return 0;
}

