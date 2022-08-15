/*#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>


int main()
{
    pid_t id = fork(); //子进程返回0，父进程返回 >0 。

    if(id == 0)
    {
        while(1)
        {
            printf("我是子进程，我的pid是：%d，我的父进程是：%d\n", getpid(), getppid());
            sleep(1);
        }
    }
    else 
    {
        while(1)
        {
            printf("我是父进程，我的pid是：%d，我的父进程是：%d\n", getpid(), getppid());
            sleep(1);
        }
    }
    return 0;
}*/ 

/*#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
int main()
{
	printf("pid: %d\n", getpid());
	printf("ppid: %d\n", getppid());
	sleep(10000);

  return 0;
}*/

/*#include<stdio.h>
#include <stdlib.h>
#include <unistd.h>

void test_orphan_process()
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
      int cnt = 5;
        while(cnt--)
        {
            printf("我是父进程，我还剩%d秒\n", cnt);
            sleep(1);
        }
        sleep(3);
        exit(0);
    }
}


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
         printf("我是子进程，我变成僵尸了\n");
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
int main()
{
  test_orphan_process();
	//test_z_p();
	return 0;
}*/
/*
#include<stdio.h>

int main(int argc, char *argv[], char *env[])
{
    int i = 0;
    for(; env[i]; ++i)
    {
        printf("%s\n", env[i]);
    }
    return 0;
}*/ 


/*
#include<stdio.h>

int main()
{
    extern char **environ;
    int i = 0;
    for(; environ[i]; ++i)
    {
        printf("%s\n", environ[i]);
    }
    return 0;
}*/ 


#include<stdio.h>
#include<stdlib.h> 
int main()
{
    char *env = getenv("MY_ENV");
    
    printf("%s\n", env);
    return 0;
}


