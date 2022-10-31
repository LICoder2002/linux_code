//客户端写入
//服务端读
#include "comm.h"

using namespace std;

int main()
{
    umask(0);
    if(mkfifo(IPC_PATH, 0600) != 0)
    {
        cerr << "mkfifo error" << endl;
        return 1;
    }

    int pipeFd = open(IPC_PATH, O_RDONLY);
    if(pipeFd < 0)
    {
        cerr << "open fifo error" << endl;
        return 2;
    }

#define NUM 1024
    char buffer[1024];

    while(true)
    {
        ssize_t s = read(pipeFd, buffer, sizeof(buffer) - 1);
        if(s > 0)
        {
            buffer[s] = '\0';
            cout << "客户端 -> 服务器# " << buffer << endl;
        }
        else if(s == 0)
        {
            break;
        }
        else
        {
            cout << "read: " << strerror(errno) << endl;
        }
    }
    close(pipeFd);
    cout << "服务端退出" << endl;
    return 0;
}