#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include<unistd.h>

//
//int main()
//{
//    pid_t pid;
//    pid = fork();
//
//    if(pid < 0)
//    {
//        printf("%s fork error", __FUNCTION__);
//        return 1;
//    }
//    else if(pid == 0)
//    {
//        printf("我是子进程，pid = %d, ppid = %d\n", getpid(), getppid());
//
//        sleep(5);
//        exit(0);
//    }
//    else 
//    {
//        int status = 0;
//        pid_t ret = waitpid(-1, &status, 0); //阻塞式等待，等待5秒
//
//        printf("我是父进程，我在等待子进程，pid = %d\n", getpid());
//        if(WIFEXITED(status) && ret == pid)
//        {
//            printf("等待子进程成功，子进程的退出码为 %d\n", WEXITSTATUS(status));
//        }
//        else 
//        {
//            printf("等待子进程失败。\n");
//            return 1;
//        }
//    }
//
//    return 0;
//
//}
//


int main()
{
    pid_t pid;
    pid = fork();

    if(pid < 0)
    {
        printf("%s fork error", __FUNCTION__);
        return 1;
    }
    else if(pid == 0)
    {
        printf("我是子进程，pid = %d, ppid = %d\n", getpid(), getppid());

        sleep(5);
        exit(1);
    }
    else 
    {
        int status = 0;

        pid_t ret = 0;

        do
        {
            ret = waitpid(-1, &status, WNOHANG);//非阻塞式等待
            if(ret == 0)
            {
                printf("子进程正在运行\n");
            }
            sleep(1);
        }while(ret ==0);


        if(WIFEXITED(status) && ret == pid)
        {
            printf("等待子进程成功，子进程的退出码为 %d\n", WEXITSTATUS(status));
        }
        else 
        {
            printf("等待子进程失败。\n");
            return 1;
        }
    }

    return 0;

}
