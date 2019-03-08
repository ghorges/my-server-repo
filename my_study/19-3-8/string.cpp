#include <iostream>
#include <string>
#include <string.h>
using namespace std;


int main()
{
    string s1 = "1111wbm hello wbm 111 wbm 222 wbm 333";

    //string find的使用
    int index = s1.find("wbm",0);
    cout << "index:" << index << endl;

    int offindex = s1.find("wbm",0);
    while(offindex != string::npos)
    {
        cout << "offindex:" << offindex << endl;
        offindex += 1;
        offindex = s1.find("wbm",offindex);
    }

    offindex = 0;
    offindex = s1.find("wbm",offindex);
    while(offindex != string::npos)
    {   
        cout << "offindex:" << offindex << endl;
        s1.replace(offindex,3,"WBM");
        offindex += strlen("WBM");
        offindex = s1.find("wbm",offindex);
    }
    
    cout << s1.c_str() << endl;
    std::cout << "Hello world" << std::endl;
    return 0;
}
