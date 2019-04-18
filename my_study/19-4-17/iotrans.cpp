#include <iostream>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


using namespace std;


int main()
{
        char buf[100] = { 0  };
        int n = 0;
        while((n = read(STDIN_FILENO,buf,100)) > 0)
                {   
                        if(write(STDOUT_FILENO,buf,n) != n)
                        {
                            perror("write err:");
                            return 0;
                        }
                }
        cout << "hello" << endl;
        return 0;
}
