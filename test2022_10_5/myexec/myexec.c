#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>


int main()
{
    //环境变量的指针声明
    extern char** environ;

    printf("我是父进程，pid = %d\n", getpid());

    pid_t id = fork();

    if(id == 0)
    {
        printf("我是子进程，pid = %d, ppid = %d\n", getpid(), getppid());

                char *const env_[] = {
            (char*)"MYPATH=YouCanSeeMe!!",
            NULL
        };
        //e: 添加环境变量给目标进程，是覆盖式的，还是新增式的！
        execle("./mycmd", "mycmd", NULL, environ);
        //execle("./mycmd", "mycmd", NULL, env_);
        
        //execl("/usr/bin/bash", "bash", "test.sh", NULL);
        //execl("/usr/bin/python3", "python3", "test.py", NULL);
        //execl("./mycmd", "mycmd", NULL);
        //execl("/home/whb/104/phase-104/lesson17/mycmd", "mycmd", NULL);
        //char *const argv_[] = {
        //    (char*)"ls",
        //    (char*)"-a",
        //    (char*)"-l",
        //    (char*)"-i",
        //    NULL
        //};
        //char *const argv_[] = {
        //    (char*)"top",
        //    NULL
        //};
        //sleep(1);
        //execvp("top", argv_);
        //execlp("ls", "ls", "-a", "-l", NULL);// 这里出现了两个ls, 含义一样吗？不一样！
        //execv("/usr/bin/top", argv_);
        //execl("/usr/bin/ls", "ls", "-a", "-l", NULL);
        exit(1); //只要执行了exit，意味着，execl系列的函数失败了
    }

    //父进程
    int status = 0;
    int ret = waitpid(id, &status, 0);
    if(ret == id)
    {
        sleep(2);
        printf("父进程等待成功！\n");
    }

    return 0;
}
