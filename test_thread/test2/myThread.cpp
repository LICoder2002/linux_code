#include <iostream>
#include <cstring>
#include <pthread.h>
#include <unistd.h>
#include <sys/syscall.h>

using namespace std;

//模拟死锁

pthread_mutex_t mutexA = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexB = PTHREAD_MUTEX_INITIALIZER;

void *startRoutine1(void *args)
{
    while (true)
    {
        pthread_mutex_lock(&mutexA);
        pthread_mutex_lock(&mutexB);
        cout << "thread 1 tid: " << pthread_self() << endl;
        pthread_mutex_unlock(&mutexA);
        pthread_mutex_unlock(&mutexB);
    }
    return nullptr;
}

void *startRoutine2(void *args)
{
    while (true)
    {
        pthread_mutex_lock(&mutexB);
        sleep(1);
        pthread_mutex_lock(&mutexA);
        cout << "thread 2 tid: " << pthread_self() << endl;
        pthread_mutex_unlock(&mutexB);
        pthread_mutex_unlock(&mutexA);
    }
    return nullptr;
}

int main()
{
    pthread_t tid1, tid2;

    pthread_create(&tid1, nullptr, startRoutine1, nullptr);
    pthread_create(&tid2, nullptr, startRoutine2, nullptr);

    pthread_join(tid1, nullptr);
    pthread_join(tid2, nullptr);
    return 0;
}

/*
int tickets = 10000;
pthread_mutex_t mutex;

void *getTickets(void *args)
{
    const char *name = static_cast<const char *>(args);
    while (true)
    {
        pthread_mutex_lock(&mutex);
        if (tickets > 0)
        {
            usleep(1000);
            cout << name << " 抢到票了，票的编号：" << tickets << endl;
            --tickets;
            pthread_mutex_unlock(&mutex);
            usleep(100);
        }
        else
        {
            cout << name << " 没有抢到票..." << endl;
            pthread_mutex_unlock(&mutex);
            break;
        }
    }
    return nullptr;
}

int main()
{
    pthread_mutex_init(&mutex, nullptr);
    pthread_t tid1;
    pthread_t tid2;
    pthread_t tid3;
    pthread_t tid4;

    pthread_create(&tid1, nullptr, getTickets, (void*)"thread 1");
    pthread_create(&tid2, nullptr, getTickets, (void*)"thread 2");
    pthread_create(&tid3, nullptr, getTickets, (void*)"thread 3");
    pthread_create(&tid4, nullptr, getTickets, (void*)"thread 4");

    sleep(1);

    //线程分离
    // pthread_detach(tid1);
    // pthread_detach(tid2);
    // pthread_detach(tid3);
    // pthread_detach(tid4);

    int n = pthread_join(tid1, nullptr);
    cout << n << ":" << strerror(errno) << endl;

    n = pthread_join(tid2, nullptr);
    cout << n << ":" << strerror(errno) << endl;

    n = pthread_join(tid3, nullptr);
    cout << n << ":" << strerror(errno) << endl;

    n = pthread_join(tid4, nullptr);
    cout << n << ":" << strerror(errno) << endl;

    return 0;
}
 */