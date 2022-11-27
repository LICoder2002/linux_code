#include <iostream>
#include <string>
#include <unistd.h>
#include <pthread.h>

using namespace std;

int global_value = 100;

static void printTid(const char *name, const pthread_t &tid)
{
    printf("%s 正在运行, thread id: 0x%x, global_value: %d\n", name, tid, global_value);
}


void *startRoutine(void *args)
{
    //pthread_detach(pthread_self());
    const char *name = static_cast<const char *>(args);
    int cnt = 5;
    while (true)
    {
        // cout << "线程的正在运行..." << endl;
        printTid(name, pthread_self());
        sleep(1);
        if (!(cnt--))
        {
            global_value = 200;
            break;
            // int *p = nullptr;
            // *p = 100; //野指针问题
        }
    }

    cout << "线程推出啦....." << endl;

    int *p = new int(10);
    return (void *)p;


    // 1. 线程退出的方式，return
    // return (void*)111;
    // 2. 线程退出的方式，pthread_exit
    // pthread_exit((void*)1111);
}

int main()
{
    pthread_t tid;

    int n = pthread_create(&tid, nullptr, startRoutine, (void*)"thread 1");
    // 3. 线程退出的方式，给线程发送取消请求， 如果线程是被取消的，退出结果是：-1
    // 3.1 main thread cancel new thread , 反过来呢？能不能，什么现象？ --- 不推荐的做法
    // pthread_cancel(tid);
    // PTHREAD_CANCELED;
    (void)n;

    //sleep(3); //代表main thread对应的工作

    //cout << "new thread been canceled" << endl;

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

}


/**
 * 创建线程
*/

/* 
void* callback1(void* args)
{
    string name = (char*)args;
    while (true)
    {
        
        cout << name << ": " << getpid() << endl;
        sleep(1);
    }
    
}

void* callback2(void* args)
{
    string name = (char*)args;
    while (true)
    {
        
        cout << name << ": " << getpid() << endl;
        sleep(1);
    }
    
}

int main()
{
    pthread_t tid1;
    pthread_t tid2;

    pthread_create(&tid1, nullptr, callback1, (void*)"thread 1");
    pthread_create(&tid1, nullptr, callback2, (void*)"thread 2");



    while (true)
    {
        cout << "main thread" << endl;
        sleep(1);
    }

    pthread_join(tid1, nullptr);
    pthread_join(tid2, nullptr);

    return 0;

} */





