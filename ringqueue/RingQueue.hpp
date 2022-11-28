#include <iostream>
#include <vector>
#include <semaphore.h>
#include <pthread.h>

using namespace std;

const uint32_t gDefaultSize = 10;

template <class T>
class RingQueue
{
public:
    RingQueue(int cap = gDefaultSize)
        : _ringQueue(cap),
          _pIndex(0),
          _cIndex(0)
    {
        sem_init(&_roomSem, 0, _ringQueue.size());
        sem_init(&_dataSem, 0, 0);

        pthread_mutex_init(&_cmutex, nullptr);
        pthread_mutex_init(&_pmutex, nullptr);
    }

    ~RingQueue()
    {
        sem_destroy(&_roomSem);
        sem_destroy(&_dataSem);

        pthread_mutex_destroy(&_cmutex);
        pthread_mutex_destroy(&_pmutex);
    }

public:
    //生产
    void push(const T &in)
    {
        //生产者申请空间，_roomSem--
        sem_wait(&_roomSem);
        pthread_mutex_lock(&_pmutex);

        _ringQueue[_pIndex] = in;
        _pIndex++;
        _pIndex %= _ringQueue.size(); //保证环形队列

        pthread_mutex_unlock(&_pmutex);

        //生产完成，_dataSem++
        sem_post(&_dataSem);
    }

    //消费
    T pop()
    {
        //消费者消费，_dataSem--
        sem_wait(&_dataSem);
        pthread_mutex_lock(&_cmutex);

        T tmp = _ringQueue[_cIndex];
        _cIndex++;
        _cIndex %= _ringQueue.size();

        pthread_mutex_unlock(&_cmutex);
        //消费完成，_roomSem++
        sem_post(&_roomSem);

        return tmp;
    }

private:
    std::vector<T> _ringQueue; //环形队列
    sem_t _roomSem;            //空间计数器 productor关心
    sem_t _dataSem;            //数据计算器 consumer关心
    uint32_t _pIndex;          //当前生产者写入资源的位置，如果是多线程，_pIndex也是临界资源
    uint32_t _cIndex;          //当前消费者消费资源的位置，如果是多线程，_cIndex也是临界资源

    pthread_mutex_t _pmutex;
    pthread_mutex_t _cmutex;
};
