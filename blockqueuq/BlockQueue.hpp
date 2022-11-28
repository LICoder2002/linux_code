#include <iostream>
#include <queue>
#include <pthread.h>
#include <unistd.h>

using namespace std;

const uint32_t gDefaultCap = 5; //默认大小

template <class T>
class BlockQueue
{

public:
    BlockQueue(uint32_t cap = gDefaultCap)
        : _cap(cap)
    {
        pthread_mutex_init(&_mutex, nullptr);
        pthread_cond_init(&_conCond, nullptr);
        pthread_cond_init(&_proCond, nullptr);
    }

    ~BlockQueue()
    {
        pthread_mutex_destroy(&_mutex);
        pthread_cond_destroy(&_conCond);
        pthread_cond_destroy(&_proCond);
    }

public:

    //生产
    void push(const T& in)
    {
        //加锁
        lockQueue();
        //队列满，等待消费者消费
        while (isFull())
        {
            proWait();
        }

        //生产
        proCore(in);
        //生产完成，唤醒消费者
        wakeupCon();
        unlockQueue();        
        
    }

    //消费
    T pop()
    {
        //加锁
        lockQueue();
        //队列为空，等待生产者生产
        while (isEmpty())
        {
            conWait();
        }

        //消费
        T tmp = popCore();
        //消费完成，唤醒生产者
        wakeupPro();
        unlockQueue();

        return tmp;
        
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

    void proWait()
    {
        pthread_cond_wait(&_proCond, &_mutex);
    }

    void conWait()
    {
        pthread_cond_wait(&_conCond, &_mutex);
    }

    void wakeupPro()
    {   
        pthread_cond_signal(&_proCond);
    }

    void wakeupCon()
    {
        pthread_cond_signal(&_conCond);
    }

    void proCore(const T& in)
    {
        _bq.push(in);
    }

    T popCore()
    {
        T tmp = _bq.front();
        _bq.pop();
        return tmp;
    }

    bool isFull()
    {
        return _bq.size() == gDefaultCap;
    }

    bool isEmpty()
    {
        return _bq.empty();
    }
private:
    queue<T> _bq;            //阻塞队列
    uint32_t _cap;           //容量
    pthread_mutex_t _mutex;  //保护阻塞队列的互斥锁
    pthread_cond_t _conCond; //让消费者等待的条件变量
    pthread_cond_t _proCond; //让生产者等待的条件变量
};