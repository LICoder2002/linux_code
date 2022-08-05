#include<iostream>
using namespace std;
#include <stdio.h>
#include <unistd.h>

int main()
{
    for(int i = 9; i >= 0; --i)
    {
      printf("%d\r", i);
      fflush(stdout);
      sleep(1);
    }
    return 0;
}
