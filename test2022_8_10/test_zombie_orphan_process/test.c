#include<stdio.h>
#include <stdlib.h>
#include <unistd.h>

void test_z_p()
{
    pid_t id = fork();
    
      if (id == 0)
      {
         int cnt = 5;
         while(cnt--)
         {
            printf("我是子进程，我还剩下%d秒\n", cnt);
            sleep(1);
         }
         printf("我变成僵尸了\n");
         exit(0);
      }
      else
      {
          while(1)
          {
          printf("我是父进程\n");
          sleep(1);
          }
      }
}

void test_orphan_p()
{
    pid_t id = fork();
      
    if(id == 0)
    {
        while(1)
        {
            printf("我是子进程\n");
            sleep(1);
        }
    }
    else
    {
      int cnt = 15;
        while(cnt--)
        {
            printf("我是父进程，我还剩%d秒\n", cnt);
            sleep(1);
        }
        sleep(3);
        exit(0);
    }
}


int main()
{
   //test_z_p();
   test_orphan_p();

    return 0;
}


