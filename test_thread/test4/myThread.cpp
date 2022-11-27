#include <iostream>
#include <vector>
#include <functional>
#include <unistd.h>
#include <pthread.h>

using namespace std;

//定义一个条件变量
pthread_cond_t cond;

//定义一个互斥锁
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

vector<function<void()>> funcs;

void show()
{
    cout << "hello show" << endl;
}

void print()
{
    cout << "hello print" << endl;
}

volatile bool quit = false;

void *waitCommand(void *args)
{

    pthread_detach(pthread_self());
    while (!quit)
    {
        // 执行了下面的代码，证明某一种条件不就绪(现在还没有场景)，要我这个线程等待
        // 三个线程，都会进在条件变量下进行排队
        pthread_cond_wait(&cond, &mutex); //让对应的线程进行等待，等待被唤醒
        for (auto &f : funcs)
        {
            f();
        }
    }
    cout << "thread id: " << pthread_self() << " end..." << endl;

    return nullptr;

    /*     pthread_detach(pthread_self());
        while (!quit)
        {
            pthread_cond_wait(&cond, &mutex);
            cout << "thread id: " << pthread_self() << "run..." << endl;
        }
        cout << "thread id: " << pthread_self() << "end..." << endl;
        return nullptr; */
}

int main()
{

    funcs.push_back(show);
    funcs.push_back(print);
    funcs.push_back([]()
                    { cout << "你好世界!" << endl; });

    pthread_cond_init(&cond, nullptr);
    pthread_t t1, t2, t3;
    pthread_create(&t1, nullptr, waitCommand, nullptr);
    pthread_create(&t2, nullptr, waitCommand, nullptr);
    pthread_create(&t3, nullptr, waitCommand, nullptr);

    while (true)
    {
        sleep(1);
        pthread_cond_broadcast(&cond);

        /*  char n = 'a';
         cout << "请输入您的command(n/q):";
         cin >> n;

         if (n == 'n')
         {
             // pthread_cond_signal(&cond);
             sleep(1);
             pthread_cond_broadcast(&cond);
         }
         else
         {
             quit = true;
             break;
         }
         sleep(1); */
    }

    cout << "main quit..." << endl;

    /*
    pthread_join(t1, nullptr);
    pthread_join(t2, nullptr);
    pthread_join(t3, nullptr);
     */

    pthread_cond_destroy(&cond);

    return 0;
}