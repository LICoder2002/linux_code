#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <cassert>

using namespace std;

void FreeChld(int signo)
{
    assert(signo == SIGCHLD);
    while (true)
    {
        // waitpid 什么时候调用失败呢？如果你已经没有子进程了
        // -1: 等待任意一个子进程
        pid_t id = waitpid(-1, nullptr, WNOHANG); // bug?？
        if (id > 0)
        {
            cout << "父进程等待成功, chld pid: " << id << endl;
        }
        else if(id == 0)
        {
            //还有子进程，但是现在没有退出
            cout << "还有子进程，但是现在没有退出, 父进程要去忙自己的事情了" << endl;
            break;
        }
        else
        {
            cout << "父进程等待所有子进程结束" << endl;
            break;
        }
    }
}

int main()
{
    // signal(SIGCHLD, FreeChld);
    // 子进程退出的时候，默认的信号处理就是忽略吗？
    // 调用signal/sigaction SIG_IGN, 意义在哪里呢？
    // SIG_IGN手动设置，让子进程退出，不要给父进程发送信号了，并且自动释放
    signal(SIGCHLD, SIG_IGN);
    for (int i = 0; i < 3; i++)
    {
        pid_t id = fork();
        if (id == 0)
        {
            //子进程
            int cnt = 3;

            // if (i < 7)
            //     cnt = 5;
            // else
            //     cnt = 20;
            while (cnt)
            {
                cout << "我是子进程, pid: " << getpid() << " 当前的cnt: " << cnt-- << endl;
                sleep(1);
            }
            cout << "子进程退出，进入僵尸状态" << endl;
            exit(0);
        }
        // sleep(1);
    }

    while (true)
    {
        cout << "我是父进程，我正在运行: " << getpid() << endl;
        sleep(1);
    }
    // //父进程,都是要自己主动等待
    // if(waitpid(id, nullptr, 0) > 0)
    // {
    //     cout << "父进程等待子进程成功" << endl;
    // }
    return 0;
}