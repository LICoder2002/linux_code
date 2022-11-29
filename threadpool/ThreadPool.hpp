#include <iostream>
#include <queue>
#include <cassert>
#include <memory>
#include <pthread.h>
#include <unistd.h>
#include <sys/prctl.h>
#include "Lock.hpp"
#include "Log.hpp"

using namespace std;

const uint32_t gDefaultNum = 5;

//改造为单例模式：

template <class T>
class ThreadPool
{
private:
    ThreadPool(uint32_t threadNum = gDefaultNum)
        : _isStart(false),
          _threadNum(threadNum)
    {
        assert(threadNum > 0);
        pthread_mutex_init(&_mutex, nullptr);
        pthread_cond_init(&_cond, nullptr);
    }

    ThreadPool(const ThreadPool<T> &) = delete;
    ThreadPool<T> &operator=(const ThreadPool<T> &) = delete;

public:
    static ThreadPool<T> *getInstance()
    {
        static Mutex mutex;
        if (nullptr == instance) //限定LockGuard的生命周期
        {
            LockGuard lockGurand(&mutex); //RAII
            if (nullptr == instance)
            {
                instance = new ThreadPool<T>();
            }
        }
        return instance;
    }

    static void *threadRoutine(void *args)
    {
        ThreadPool<T> *ptp = static_cast<ThreadPool<T> *>(args);

        while (true)
        {
            ptp->lockQueue();
            //判断当前任务队列有没有任务
            while (!ptp->hasTask())
            {
                //没有任务，当前线程等待
                ptp->waitTask();
            }

            //当前线程处理任务
            T t = ptp->pop();

            ptp->unlockQueue();

            Log() << "新线程完成任务：" << t.run() << endl;
        }
    }

    void start()
    {
        assert(!_isStart); //判断线程是否已经启动
        for (int i = 0; i < _threadNum; ++i)
        {
            pthread_t tid;
            pthread_create(&tid, nullptr, threadRoutine, this);
        }
        _isStart = true;
    }

    //放任务
    void push(const T &in)
    {
        lockQueue();
        _taskQueue.push(in);
        choiceThreadHandle();
        unlockQueue();
    }

    //消费任务
    T pop()
    {
        T tmp = _taskQueue.front();
        _taskQueue.pop();
        return tmp;
    }

    ~ThreadPool()
    {
        pthread_mutex_destroy(&_mutex);
        pthread_cond_destroy(&_cond);
    }

private:
    void lockQueue()
    {
        pthread_mutex_lock(&_mutex);
    }

    void unlockQueue()
    {
        pthread_mutex_unlock(&_mutex);
    }

    void waitTask()
    {
        pthread_cond_wait(&_cond, &_mutex);
    }

    void choiceThreadHandle()
    {
        pthread_cond_signal(&_cond);
    }

    bool hasTask()
    {
        return !_taskQueue.empty();
    }

private:
    bool _isStart;          //判断线程池是否开启
    uint32_t _threadNum;    //线程池中线程数量
    queue<T> _taskQueue;    //任务队列
    pthread_mutex_t _mutex; //保护任务队列的锁
    pthread_cond_t _cond;   //线程的条件变量

    static ThreadPool<T> *instance;
};

template <class T>
ThreadPool<T> *ThreadPool<T>::instance = nullptr;

/*
template <class T>
class ThreadPool
{

public:
    ThreadPool(uint32_t threadNum = gDefaultNum)
        : _isStart(false),
          _threadNum(threadNum)
    {
        assert(threadNum > 0);
        pthread_mutex_init(&_mutex, nullptr);
        pthread_cond_init(&_cond, nullptr);
    }


    static void* threadRoutine(void* args)
    {
        ThreadPool<T>* ptp = static_cast<ThreadPool<T>*>(args);

        while (true)
        {
            ptp->lockQueue();
            //判断当前任务队列有没有任务
            while(!ptp->hasTask())
            {
                //没有任务，当前线程等待
                ptp->waitTask();
            }

            //当前线程处理任务
            T t = ptp->pop();

            ptp->unlockQueue();

            Log() << "新线程完成任务：" << t.run() << endl;
        }

    }

    void start()
    {
        assert(!_isStart); //判断线程是否已经启动
        for (int i = 0; i < _threadNum; ++i)
        {
            pthread_t tid;
            pthread_create(&tid, nullptr, threadRoutine, this);
        }
        _isStart = true;
    }

    //放任务
    void push(const T& in)
    {
        lockQueue();
        _taskQueue.push(in);
        choiceThreadHandle();
        unlockQueue();

    }

    //消费任务
    T pop()
    {
        T tmp = _taskQueue.front();
        _taskQueue.pop();
        return tmp;
    }

    ~ThreadPool()
    {
        pthread_mutex_destroy(&_mutex);
        pthread_cond_destroy(&_cond);
    }


private:
    void lockQueue()
    {
        pthread_mutex_lock(&_mutex);
    }

    void unlockQueue()
    {
        pthread_mutex_unlock(&_mutex);
    }

    void waitTask()
    {
        pthread_cond_wait(&_cond, &_mutex);
    }

    void choiceThreadHandle()
    {
        pthread_cond_signal(&_cond);
    }

    bool hasTask()
    {
        return !_taskQueue.empty();
    }

private:
    bool _isStart;          //判断线程池是否开启
    uint32_t _threadNum;    //线程池中线程数量
    queue<T> _taskQueue;    //任务队列
    pthread_mutex_t _mutex; //保护任务队列的锁
    pthread_cond_t _cond;   //线程的条件变量
}; */