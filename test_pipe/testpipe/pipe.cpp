#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/wait.h>
using namespace std;

int main()
{
    int pipefd[2] = {0};

    if(pipe(pipefd) != 0)
    {
        cerr << "pipe error" << endl;
        return 1;
    } 

    pid_t id = fork();
    if(id < 0)
    {
        cerr << "fork error" << endl;
        return 2;
    }
    else if(id == 0)
    {
        //子进程读，关闭pipefd[1]
        close(pipefd[1]);
        char buffer[1024];
        while (true)
        {
            memset(buffer, 0, sizeof(buffer));
            ssize_t s = read(pipefd[0], buffer, sizeof(buffer)); 
            if(s > 0)
            {
                sleep(1);
                //buffer[s] = '\0';
                cout << "子进程收到父进程的消息：" << buffer << endl;
            }
            else if(s == 0)
            {
                break;
            }
            else
            {
                //...
            }

        }
        


        close(pipefd[0]);
    }
    else
    {
        //父进程写，关闭pipefd[0]
        close(pipefd[0]);
        const char* msg = "I am father";
        
        write(pipefd[1], msg, strlen(msg));
        cout << "父进程写完了" << endl;


        close(pipefd[1]);
    }

    pid_t res = waitpid(id, nullptr, 0);
    if(res > 0)
    {
        cout << "等待子进程成功" << endl;
    }

    return 0;
    
}