#include <iostream>
#include <vector>
#include <unistd.h>
#include <cstring>
#include <ctime>
#include <unordered_map>
#include <string>
#include <sys/wait.h>
#include <cassert>


using namespace std;
typedef void (*functor)(); //函数指针

vector<functor> functors;

unordered_map<uint32_t, string> info;

void f1()
{
    cout << "这是一个处理日志的任务，执行的进程 ID [" << getpid() << "]" << "执行的时间是[" 
    << time(nullptr) << "]\n" << endl;
}

void f2()
{
    cout << "这是一个备份数据任务，执行的进程 ID [" << getpid() << "]"
         << "执行时间是[" << time(nullptr) << "]\n" << endl;
}
void f3()
{
    cout << "这是一个处理网络连接的任务，执行的进程 ID [" << getpid() << "]"
         << "执行时间是[" << time(nullptr) << "]\n" << endl;
}

void loadFunctor()
{
    functors.push_back(f1);
    info.insert({functors.size(), "处理日志的任务"});
    
    functors.push_back(f2);
    info.insert({functors.size(), "备份数据任务"});

    functors.push_back(f3);
    info.insert({functors.size(), "处理网络连接的任务"});

}

// first: 进程pid，second：该进程对应的管道写端fd
typedef std::pair<int32_t, int32_t> elem;
int processNum = 5; //创建的子进程数

void work(int blockfd)
{
    cout << "进程[" << getpid() << "]开始工作"  << endl;

    while(true)
    {
        //子进程做的工作
        //a. 阻塞等待
        //b. 获取任务信息
        uint32_t operatorCode = 0;
        ssize_t s = read(blockfd, &operatorCode, sizeof(uint32_t));
        if(s == 0) break;
        assert(s == sizeof(uint32_t));
        (void) s; //消除realease模式的告警

        //c. 处理任务
        if(operatorCode < functors.size())
            functors[operatorCode]();
    }
    cout << "进程[" << getpid() << " ]结束工作"  << endl;
    
}
// elem: first: 进程pid，second：该进程对应的管道写端fd
void sendTask(const vector<elem>& processFds)
{
    srand((long long)time(nullptr));

    while(true)
    {
        sleep(2);
            //选择一个进程
    uint32_t pick = rand() % processFds.size();

    //选择一个任务
    uint32_t task = rand() % functors.size();

    //把任务交给一个进程
    write(processFds[pick].second, &task, sizeof(task));

    cout << "父进程指派任务->" << info[task] << "给进程: " << processFds[pick].first << " 编号: " << pick << endl;
    }

}

//父进程控制多个子进程
/*
int main()
{
    loadFunctor();

    vector<elem> assignMap;

    //创建子进程
    for(int i = 0; i < processNum; ++i)
    {
        int pipefd[2] = {0};
        //创建管道
        pipe(pipefd);
        //创建子进程
        pid_t id = fork();
        if(id == 0)
        {
            //子进程读取 pipefd[0]
            close(pipefd[1]);
            //子进程执行
            work(pipefd[0]);

            close(pipefd[0]);
            exit(0);
        }

        //父进程的工作
        close(pipefd[0]);
        elem e(id, pipefd[1]);
        assignMap.push_back(e);
    }

    //父进程分配任务
    sendTask(assignMap);

    //回收资源
    for(int i = 0; i < processNum; ++i)
    {
        if(waitpid(assignMap[i].first, nullptr, 0) > 0)
        {
            cout << "wait for: pid = " << assignMap[i].first << " success!" 
            << "number: " << i << endl;

            close(assignMap[i].second);
        }
    }

}*/

//演示管道通信的过程————匿名管道
int main()
{

    //创建匿名管道
    int pipefd[2] = {0};
    if(pipe(pipefd) != 0)
    {
        cerr << "pipe error" << endl;
        return 1;
    }

    //创建子进程
    pid_t id = fork();
    if(id < 0)
    {
        cerr << "fork error" << endl;
        return 2;
    }
    else if(id == 0)
    {
        //子进程进行读操作，应该关闭写
        close(pipefd[1]);
        #define NUM 1024
        char buffer[NUM];
        while(true)
        {
            cout << "时间戳：" <<(uint64_t)time(nullptr) << endl;

            memset(buffer, 0, sizeof(buffer));
            ssize_t s = read(pipefd[0], buffer, sizeof(buffer) - 1);
            if(s > 0)
            {
                buffer[s] = '\0';
                cout << "子进程收到消息，内容是：" << buffer << endl;
            }
            else if(s == 0)
            {
                cout << "父进程写完了，子进程退出" << endl;
                break;
            }
            else
            {
                //do nothing
            }
        }
        close(pipefd[0]);
        exit(0);

    }
    else
    {
        //父进程进行写操作，应该关闭读
        close(pipefd[0]);
        const char* msg = "你好啊，子进程！我是父进程。这次发送信息的编号：";

        int cnt = 0;
        while(cnt < 5)
        {
            char sendBuffer[1024];
            sprintf(sendBuffer, "%s : %d", msg, cnt);

            sleep(1);
            write(pipefd[1], sendBuffer, strlen(msg));
            
            ++cnt;
            
            cout << "cnt:" << cnt <<endl;
        }

        close(pipefd[1]);
        cout << "父进程写完了" <<endl;

    }

}
