//客户端写入
#include "comm.h"

using namespace std;

int main()
{
    int pipeFd = open(IPC_PATH, O_WRONLY);
    if(pipeFd < 0)
    {
        cerr << "open: " << strerror(errno) << endl;
        return 1;
    }

#define NUM 1024
    char line[NUM];
    while(true)
    {
        printf("请输入你的消息# ");
        fflush(stdout);
        memset(line, 0, sizeof(line));

        if(fgets(line, sizeof(line), stdin) != nullptr)
        {
            line[strlen(line) - 1] = '\0'; // abcd\n\0
            write(pipeFd, line, strlen(line));
        }
        else
        {
            break;
        }
    }
    close(pipeFd);
    cout << "客户端退出" << endl;
    
}