#include <iostream>
using namespace std;
int main()
{
    int l = 0;
    cout << "请输入长度" << endl;
    cin >> l;
    char c[1000] = { 0 };
    cout << "请输入字符串" << endl;
    cin >> c;
    int sum = 0;
    int sum_aa = 0;
    for(int i = 1;i < l;i++)
    {
        if(c[i] != c[i - 1])
        {
            if(sum_aa == 0)
            {
                sum_aa = 1;
            }
            sum_aa++;
        }
        else
        {
            if(sum < sum_aa)
            {
                sum = sum_aa;
            }
            sum_aa = 0;
        }
    }
    if(sum_aa > sum)
    {
        sum = sum_aa;
    }
    cout << sum << endl;
    std::cout << "Hello world" << std::endl;
    return 0;
}

