#include "comm.h"

using namespace std;

#define NUM 1024

int main()
{
    //打开命名管道
    int pipeFd = open(IPC_PATH, O_WRONLY);
    if(pipeFd < 0)
    {
        cerr << "open error" << endl;
        return 2; 
    }

    //向命名管道中写入数据

    char line[NUM];
    while(true)
    {
        printf("请输入你的消息# ");
        fflush(stdout);
        memset(line, 0, sizeof(line));

        //fgets得到C风格字符串，会在末尾自动添加 '\0'
        if(fgets(line, sizeof(line), stdin) != nullptr)
        {
            
            line[strlen(line) - 1] = '\0'; //处理回车  例如输入 abcd/n/0 
            write(pipeFd, line, strlen(line));
        }
        else
        {
            break;
        }
    }

    close(pipeFd);
    cout << "客户端退出" << endl;
    return 0;

}