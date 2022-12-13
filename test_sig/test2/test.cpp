/* #include <iostream>
#include <cstring>
#include <signal.h>
#include <unistd.h>

using namespace std; */

/* void handler(int signo)
{
    cout << "我是一个进程，刚刚获取了一个信号: " << signo << endl;
    // exit(1);
}

static void showPendings(sigset_t *pendings)
{
    for (int sig = 1; sig <= 31; ++sig)
    {
        if (sigismember(pendings, sig))
        {
            cout << "1";
        }
        else
        {
            cout << "0";
        }
    }
    cout << endl;
}

int main()
{
    cout << "pid = " << getpid() << endl;

    // 3. 屏蔽2号信号
    sigset_t bsig, obsig;
    sigemptyset(&bsig);
    sigemptyset(&obsig);

    for (int sig = 1; sig <= 31; sig++)
    {
        // 3.1 添加信号到信号屏蔽字中
        sigaddset(&bsig, sig);

        // 2. signal
        signal(sig, handler);
    }

    // 3.2 设置用户集的信号屏蔽字到内核中，让当前进程屏蔽掉2号信号
    sigprocmask(SIG_SETMASK, &bsig, &obsig);

    // 1. 不断获取当前进程的pending信号集
    sigset_t pendings;
    int cnt = 0;
    while (true)
    {
        // 1.1 清空信号集
        sigemptyset(&pendings);
        // 1.2 获取当前进程的pending信号集
        if (sigpending(&pendings) == 0)
        {
            // 1.3 打印当前pending信号集的内容
            showPendings(&pendings);
        }

        sleep(1);
        cnt++;
        if (cnt == 20)
        {
            cout << "解除对所有信号的block..." << endl;
            sigset_t sigs;
            sigemptyset(&sigs);
            sigaddset(&sigs, 2);
            sigprocmask(SIG_UNBLOCK, &sigs, nullptr);
        }
    }

    return 0;
} */
/*
void handler(int signo)
{
    cout << "获取到一个信号,信号的编号是: " << signo << endl;
    sigset_t pending;
     //增加handler信号的时间,永远都会正在处理2号信号！
    while (true)
    {
        cout << "." << endl;
        sigpending(&pending);
        for(int i = 1; i <=31; i++){
            if(sigismember(&pending, i)) cout << '1';
            else cout << '0';
        }
        cout << endl;
        sleep(1);
    }
}

void Handler2()
{
    cout << "hello 2" << endl;
}
void Handler3()
{
    cout << "hello 3" << endl;
}
void Handler4()
{
    cout << "hello 4" << endl;
}


void Handler5()
{
    cout << "hello 5" << endl;
}
void Handler(int signo)
{
    switch (signo)
    {
    case 2:
        Handler2();
        break;
    case 3:
        Handler3();
        break;
    case 4:
        Handler4();
        break;
    case 5:
        Handler5();
        break;
    default:
        break;
    }
}

int main()
{
    cout << "pid = " << getpid() << endl;
    signal(2, Handler);
    signal(3, Handler);
    signal(4, Handler);
    signal(5, Handler);

    // while(1)
    // {
    //     sleep(1);
    // }

    struct sigaction act, oact;
    act.sa_handler = handler;
    // act.sa_handler = SIG_IGN;
    // act.sa_handler = SIG_DFL;
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);
    sigaddset(&act.sa_mask, 3);
    sigaction(2, &act, &oact);

    while (true)
    {
        cout << "main running" << endl;
        sleep(1);
    }
    return 0;
} */

/* struct sigaction act, oact;
void handler(int signo)
{
    printf("get a signal:%d\n", signo);
    sigaction(2, &oact, NULL);
}
int main()
{
    cout << "pid = " << getpid() << endl;

    memset(&act, 0, sizeof(act));
    memset(&oact, 0, sizeof(oact));

    act.sa_handler = handler;
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);

    sigaction(2, &act, &oact);
    while (1)
    {
        cout << "main running" << endl;

        sleep(1);
    }
    return 0;
}
 */

/* #include <stdio.h>
#include <signal.h>

// 保持内存的可见性
int flags = 0;

void handler(int signo)
{
    flags = 1;
    printf("更改flags: 0->1\n");
}

int main()
{
    // 可以同时修饰一个变量吗？？
    // 含义冲突吗，该变量代表什么含义？
    //volatile const int a = 100;

    signal(2, handler);

    while (!flags);
    printf("进程是正常退出的!\n");

    return 0;
} */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

void handler(int sig)
{
    pid_t id;
    while ((id = waitpid(-1, NULL, WNOHANG)) > 0)
    {
        printf("wait child success: %d\n", id);
    }
    printf("child is quit! %d\n", getpid());
}
int main()
{
    signal(SIGCHLD, handler);
    pid_t cid;
    if ((cid = fork()) == 0)
    { // child
        printf("child : %d\n", getpid());
        sleep(3);
        exit(1);
    }
    while (1)
    {
        printf("father proc is doing some thing!\n");
        sleep(1);
    }
    return 0;
}