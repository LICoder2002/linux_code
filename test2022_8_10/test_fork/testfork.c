#include<stdio.h>
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
}
