#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

using namespace std;

int cnt = 0;

void handler(int signo)
{
    cout << "我是一个进程，刚刚获取了一个信号： " << signo << endl;
    sleep(5);
}



int main()
{
    signal(SIGSEGV, handler);

    sleep(1);

    int *p = nullptr;
    *p = 1000;
    
    //while (true);

    return 0;
}

/* int main()
{
    signal(SIGALRM, handler);

    alarm(1); //设置1s

    while (true)
    {
        cnt++;
        //printf("cnt = %d\n", cnt);
    }
    return 0;
}
 */



/*
int main()
{
    int fd[2] = {0};

    if (pipe(fd) < 0)
    {
        cerr << "pipe error" << endl;
        exit(1);
    }

    pid_t id = fork();
    if (id == 0)
    {
        //子进程 --- 写端
        close(fd[0]);

        const char *msg = "父进程你好，我是子进程...";

        int count = 5;
        while (count)
        {
            write(fd[1], msg, strlen(msg));
            sleep(1);
            count--;
        }

        close(fd[1]);
        exit(0);
    }

    //父进程 --- 读端
    close(fd[1]);

    close(fd[0]); //父进程关闭自己的写端，导致子进程写入会被操作系统终止

    int status = 0;
    waitpid(id, &status, 0);

    cout << "子进程收到信号：" << (status & 0x7F) << endl;
    return 0;
}*/
