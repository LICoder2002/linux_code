#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include <sys/wait.h>

int main()
{
    pid_t id = fork();
    if(id==0)
    {
      int cnt = 5;//子进程运行5秒
        while(1)
        {
            printf("我是子进程，我正在运行..., pid = %d\n", getpid());
           sleep(1);
           //  break;
           cnt--;
           if(!cnt)
           {
               break;
           }
        }
        exit(1);
    }
    else 
    {
        int status = 0;
        printf("我是父进程 pid = %d，我正在等待子进程结束！\n", getpid());

        pid_t ret = waitpid(id, &status, 0);

        printf("等待成功，ret = %d，我所等待的子进程退出码为：%d, 退出信号是：%d\n", ret, (status>>8)&0xFF, status&0x7F);
    }

    return 0;


}
