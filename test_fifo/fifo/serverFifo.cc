#include "comm.h"

using namespace std;
#define NUM 1024

int main()
{
    //创建命名管道文件
    umask(0);
    if (mkfifo(IPC_PATH, 0600) != 0)
    {
        cerr << "mkfifo error" << endl;
        return 1;
    }

    //打开命名管道文件
    int pipeFd = open(IPC_PATH, O_RDONLY);
    if (pipeFd < 0)
    {
        cerr << "open error" << endl;
        return 2;
    }

    //向命名管道中读取数据

    char buffer[NUM];
    while (true)
    {
        
        ssize_t s = read(pipeFd, buffer, sizeof(buffer) - 1);
        if (s > 0)
        {
            buffer[s] = '\0'; 
            cout << "客户端 -> 服务端# " << buffer << endl;
        }
        else if (s == 0)
        {
            cout << "客户端退出了，服务端也退出！" << endl;
            break;
        }
        else
        {
            //读取数据错误
            cout << "read: " << strerror(errno) << endl;
            break;
        }
    }

    close(pipeFd);
    cout << "服务器退出" << endl;
    unlink(IPC_PATH);
    return 0;
}