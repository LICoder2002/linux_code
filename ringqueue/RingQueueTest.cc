#include "RingQueue.hpp"
#include <ctime>
#include <unistd.h>

const std::string ops = "+-*/%";

void *consumer(void *args)
{
    RingQueue<int> *prq = static_cast<RingQueue<int> *>(args);

    while (true)
    {
        sleep(1);

        int data = prq->pop();

        cout << "consumer【" << pthread_self() << "】" << (unsigned long)time(nullptr) << "消费了一个数据：" << data << endl;
    }
}

void *productor(void *args)
{
    RingQueue<int> *prq = static_cast<RingQueue<int> *>(args);

    while (true)
    {
        int data = rand() % 20;
        prq->push(data);

        cout << "productor【" << pthread_self() << "】" << (unsigned long)time(nullptr) << "生产了一个数据：" << data << endl;

        //sleep(1);
    }
}

int main()
{
    srand((unsigned long)time(nullptr) ^ getpid());
    RingQueue<int> rq;

    //创建生产者、消费者线程
    pthread_t c1,c2,c3,c4,c5, p1,p2,p3,p4,p5;
    pthread_create(&c1, nullptr, consumer, &rq);
    pthread_create(&c2, nullptr, consumer, &rq);
    pthread_create(&c3, nullptr, consumer, &rq);
    pthread_create(&c4, nullptr, consumer, &rq);
    pthread_create(&c5, nullptr, consumer, &rq);
    pthread_create(&p1, nullptr, productor, &rq);
    pthread_create(&p2, nullptr, productor, &rq);
    pthread_create(&p3, nullptr, productor, &rq);
    pthread_create(&p4, nullptr, productor, &rq);
    pthread_create(&p5, nullptr, productor, &rq);

    pthread_join(c1, nullptr);
    pthread_join(c2, nullptr);
    pthread_join(c3, nullptr);
    pthread_join(c4, nullptr);
    pthread_join(c5, nullptr);
    pthread_join(p1, nullptr);
    pthread_join(p2, nullptr);
    pthread_join(p3, nullptr);
    pthread_join(p4, nullptr);
    pthread_join(p5, nullptr);

    return 0;
}
