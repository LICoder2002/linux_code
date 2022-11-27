#include <iostream>
#include <cstring>
#include <pthread.h>
#include <unistd.h>
#include "Lock.hpp"
using namespace std;

int tickets = 10;

Mutex myMutex;

static int cnt = 0;
bool getTickets()
{

    bool ret = false;
    LockGuard LockGuard(&myMutex);
    if (tickets > 0)
    {
        usleep(1000);
        cout << pthread_self() << " 抢到票了，票的编号：" << tickets << endl;
        --tickets;
        ret = true;
    }
    cnt++;
    return ret;
}

void *startRoutine(void *args)
{
    const char *name = static_cast<const char *>(args);
    while (true)
    {
        if (!getTickets())
        {
            break;
        }
        cout << name << "抢票成功！" << endl;

        //...
        sleep(1);
    }
}

int main()
{

    pthread_t tid1;
    pthread_t tid2;
    pthread_t tid3;
    pthread_t tid4;

    pthread_create(&tid1, nullptr, startRoutine, (void *)"thread 1");
    pthread_create(&tid2, nullptr, startRoutine, (void *)"thread 2");
    pthread_create(&tid3, nullptr, startRoutine, (void *)"thread 3");
    pthread_create(&tid4, nullptr, startRoutine, (void *)"thread 4");

    sleep(1);

    int n = pthread_join(tid1, nullptr);
    cout << n << ":" << strerror(errno) << endl;

    n = pthread_join(tid2, nullptr);
    cout << n << ":" << strerror(errno) << endl;

    n = pthread_join(tid3, nullptr);
    cout << n << ":" << strerror(errno) << endl;

    n = pthread_join(tid4, nullptr);
    cout << n << ":" << strerror(errno) << endl;

    cout << "getTickets函数调用次数: " << cnt << endl;
    return 0;
}
