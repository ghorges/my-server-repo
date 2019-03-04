#include <iostream>

using namespace std;


int main()
{
    double m;
    cout << "请输入你要兑换钱的个数：";
    cin >> m;
    int t = static_cast<int>(m);
    m -= t;
        if(m >= 0.5)
        {
            m -= 0.5;
            t++;
        }

        if(m >= 0.2)
        {
            m -= 0.2;
            t++;
            if(m >= 0.2)
            {
                m -= 0.2;
                t++;
            }
        }
    if(m >= 0.1)
    {
        m -= 0.1;
        t++;
    }
    cout << t << endl;
    return 0;
}

