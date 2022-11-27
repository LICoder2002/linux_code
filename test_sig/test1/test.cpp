#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

using namespace std;


/*
int main()
{
    pid_t id = fork();

    if (id == 0)
    {
        //子进程
        int *p = nullptr;
        *p = 1000;

        exit(1);
    }

    //父进程
    int status = 0;
    waitpid(id, &status, 0);

    printf("exit code %d, sigo: %d, core dump flag: %d\n", (status >> 8) & 0xFF, (status >> 7) & 0x7F, (status >> 7) & 0x1);

    return 0;
}

*/


int main()
{
    while (1){
        printf("hello signal! pid = %d\n", getpid());
        sleep(1);
    }
    return 0;
}


/*
int main()
{
    printf("Hello Linux...\n");

    sleep(3);
    int a = 1/0;

    return 0;
}
*/


