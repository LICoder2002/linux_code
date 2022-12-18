/* #include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <pthread.h>

using namespace std;

int global_value = 100;

static void printTid(const char *name, const pthread_t &tid)
{
    printf("%s 正在运行, thread id: 0x%x, global_value: %d\n", name, tid, global_value);
}

void *startRoutine1(void *args)
{
    // pthread_detach(pthread_self());
    const char *name = static_cast<const char *>(args);
    int cnt = 5;

    while (cnt--)
    {
        printTid(name, pthread_self());
    }

    // while (true)
    // {
    //     // cout << "线程的正在运行..." << endl;
    //     printTid(name, pthread_self());
    //     sleep(1);
    //     if (!(cnt--))
    //     {
    //         //global_value = 200;
    //         break;
    //         // int *p = nullptr;
    //         // *p = 100; //野指针问题
    //     }
    // }

    cout << "线程退出啦....." << endl;

    // int *p = new int(10);
    // return (void *)p;

    // 1. 线程退出的方式，return
    // return (void*)111;
    // 2. 线程退出的方式，pthread_exit
    pthread_exit((void *)1111);
}

void *startRoutine(void *args)
{
    const char *name = static_cast<const char *>(args);
    int cnt = 3;

    while (cnt--)
    {
        cout << "I am " << name << endl;
        sleep(1);
    }
    //pthread_exit((void*)111);
}

int main()
{
    pthread_t tid;

    int n = pthread_create(&tid, nullptr, startRoutine, (void *)"thread 1");
    (void)n;

    //sleep(1);
    void *ret = nullptr;     // void* -> 64 -> 8byte -> 空间

    //pthread_join(tid, &ret); // void **retval是一个输出型参数

    cout << "main thread join success, *ret: " << *((int *)ret) << endl;
    //delete (int *)ret;
    return 0;
} */

/*
int main()
{
    pthread_t tid;

    int n = pthread_create(&tid, nullptr, startRoutine, (void*)"thread 1");
    // 3. 线程退出的方式，给线程发送取消请求， 如果线程是被取消的，退出结果是：-1
    // 3.1 main thread cancel new thread , 反过来呢？能不能，什么现象？ --- 不推荐的做法
    // pthread_cancel(tid);
    // PTHREAD_CANCELED;
    //(void)n;

    // sleep(3); //代表main thread对应的工作

    // cout << "new thread been canceled" << endl;

    // sleep(10);
    //  线程退出的时候，一般必须要进行join，如果不进行join，就会
    //  造成类似于进程那样的内存泄露问题

    //sleep(10);

    //printTid(tid);
    // cout << "new thread id : " << tid << endl; //线程ID -- 为什么这么大？
    // while (true)
    // {
    //     printTid("main thread", pthread_self());
    //     // cout << "main thread 正在运行..." << endl;
    //     sleep(1);
    // }



    sleep(1);
    void *ret = nullptr;     // void* -> 64 -> 8byte -> 空间
    pthread_join(tid, &ret); // void **retval是一个输出型参数
    cout << "main thread join success, *ret: " << *((int*)ret) << endl;
    delete (int*)ret;

} */

/**
 * 创建线程
 */
/*
void *callback1(void *args)
{
    string name = (char *)args;
    while (true)
    {

        cout << "I am " << name << endl;
        sleep(1);
    }
}

void *callback2(void *args)
{
    string name = (char *)args;
    while (true)
    {

        cout << "I am " << name << endl;

        sleep(1);
    }
}

int main()
{
    pthread_t tid1;
    pthread_t tid2;

    if(pthread_create(&tid1, nullptr, callback1, (void *)"thread 1") != 0)
    {
        cout << "pthread_create thread 1: " << strerror(errno) << endl;
    }

    if(pthread_create(&tid2, nullptr, callback2, (void *)"thread 2") != 0)
    {
        cout << "pthread_create thread 2: " << strerror(errno) << endl;
    }

    while (true)
    {
        cout << "I am main thread" << endl;
        sleep(1);
    }

    pthread_join(tid1, nullptr);
    pthread_join(tid2, nullptr);

    return 0;
}
 */

/* #include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
void *thread1(void *arg)
{
    printf("thread 1 returning ... \n");
    int *p = (int *)malloc(sizeof(int));
    *p = 1;
    return (void *)p;
}

void *thread2(void *arg)
{
    printf("thread 2 exiting ...\n");
    int *p = (int *)malloc(sizeof(int));
    *p = 2;
    pthread_exit((void *)p);
}
void *thread3(void *arg)
{
    while (1)
    {
        printf("thread 3 is running ...\n");
        sleep(1);
    }
    return NULL;
}
int main(void)
{
    pthread_t tid;
    void *ret;
    // thread 1 return
    pthread_create(&tid, NULL, thread1, NULL);
    pthread_join(tid, &ret);
    printf("thread return, thread id %X, return code:%d\n", tid, *(int *)ret);
    free(ret);
    // thread 2 exit
    pthread_create(&tid, NULL, thread2, NULL);
    pthread_join(tid, &ret);
    printf("thread return, thread id %X, return code:%d\n", tid, *(int *)ret);
    free(ret);

    // thread 3 cancel by other
    pthread_create(&tid, NULL, thread3, NULL);
    sleep(3);
    pthread_cancel(tid);
    pthread_join(tid, &ret);
    if (ret == PTHREAD_CANCELED)
        printf("thread return, thread id %X, return code:PTHREAD_CANCELED\n", tid);
    else
        printf("thread return, thread id %X, return code:NULL\n", tid);

    return 0;
} */

/* #include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

void *thread_run(void *arg)
{
    pthread_detach(pthread_self());
    printf("%s\n", (char *)arg);
    return NULL;
}
int main(void)
{
    pthread_t tid;
    if (pthread_create(&tid, NULL, thread_run, (void*)"thread1 run...") != 0)
    {
        printf("create thread error\n");
        return 1;
    }

    int ret = 0;
    sleep(1); // 很重要，要让线程先分离，再等待
    if (pthread_join(tid, NULL) == 0)
    {
        printf("pthread wait success\n");
        ret = 0;
    }
    else
    {
        printf("pthread wait failed\n");
        ret = 1;
    }
    return ret;
} */
/*
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
int ticket = 20;
void *route(void *arg)
{
    char *id = (char *)arg;
    while (1)
    {
        if (ticket > 0)
        {
            usleep(1000);
            printf("%s sells ticket:%d\n", id, ticket);
            ticket--;
        }
        else
        {
            break;
        }
    }
}
int main(void)
{
    pthread_t t1, t2, t3, t4;
    pthread_create(&t1, NULL, route, (void *)"thread 1");
    pthread_create(&t2, NULL, route, (void *)"thread 2");
    pthread_create(&t3, NULL, route, (void *)"thread 3");
    pthread_create(&t4, NULL, route, (void *)"thread 4");
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    pthread_join(t4, NULL);

    return 0;
} */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sched.h>
int ticket = 25;

pthread_mutex_t mutex;

void *route(void *arg)
{
    char *id = (char *)arg;
    while (1)
    {
        pthread_mutex_lock(&mutex);
        if (ticket > 0)
        {
            usleep(1000);
            printf("%s sells ticket:%d\n", id, ticket);
            ticket--;
            pthread_mutex_unlock(&mutex);
            // sched_yield(); 放弃CPU
        }
        else
        {
            pthread_mutex_unlock(&mutex);
            break;
        }
    }
}
int main(void)
{
    pthread_t t1, t2, t3, t4;
    pthread_mutex_init(&mutex, NULL);
    pthread_create(&t1, NULL, route, (void *)"thread 1");
    pthread_create(&t2, NULL, route, (void *)"thread 2");
    pthread_create(&t3, NULL, route, (void *)"thread 3");
    pthread_create(&t4, NULL, route, (void *)"thread 4");

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    pthread_join(t4, NULL);
    pthread_mutex_destroy(&mutex);
}