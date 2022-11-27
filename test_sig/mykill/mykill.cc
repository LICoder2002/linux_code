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
    cout << "我是一个进程，刚刚获取了一个信号： " << signo << " cnt: " << cnt << endl;
}

static void Usage(const string &proc)
{
    cerr << "Usage:\n\t" << proc << " signo pid" << endl;
}



int main()
{
    signal(2, handler);

    while(true)
    {
        sleep(1);
        cnt++;

    
        abort();
    }
    return 0;
}

/* int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        Usage(argv[0]);
        exit(1);
    }

    if (kill(static_cast<pid_t>(atoi(argv[2])), atoi(argv[1])) == 1)
    {
        cerr << " kill: " << strerror(errno) << endl;
    }

    return 0;
} */

