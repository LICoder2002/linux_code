#include "BlockQueue.hpp"
#include "Task.hpp"
#include <ctime>

const std::string ops = "+-*/%";

void *consumer(void *args)
{
    BlockQueue<Task> *pbq = static_cast<BlockQueue<Task> *>(args);

    while (true)
    {
        Task t = pbq->pop();
        int res = t();
        int elem1, elem2;
        char oper;
        t.get(&elem1, &elem2, &oper);

        cout << "consumer【" << pthread_self() << "】" << (unsigned long)time(nullptr) << "消费了一个任务：" << elem1 << oper << elem2 << " = " << res << endl;
        //sleep(1);
    }
}

void *productor(void *args)
{
    BlockQueue<Task> *pbq = static_cast<BlockQueue<Task> *>(args);

    while (true)
    {
        int elem1 = rand() % 50;
        int elem2 = rand() % 50;
        char oper = ops[rand() % ops.size()];

        pbq->push(Task(elem1, elem2, oper));
        cout << "productor【" << pthread_self() << "】" << (unsigned long)time(nullptr) << "生产了一个任务：" << elem1 << oper << elem2 << " = "
             << "?" << endl;

        sleep(1);
    }
}

int main()
{
    srand((unsigned long)time(nullptr) ^ getpid());
    BlockQueue<Task> bq;

    //创建生产者、消费者线程
    pthread_t c, p;
    pthread_create(&c, nullptr, consumer, &bq);
    pthread_create(&p, nullptr, productor, &bq);

    pthread_join(c, nullptr);
    pthread_join(p, nullptr);

    return 0;
}

// void *consumer(void *args)
// {
//     BlockQueue<int> *pbq = static_cast<BlockQueue<int> *>(args);

//     while (true)
//     {
//         int res = pbq->pop();
//         cout << "消费者消费了一个数据：" << res << endl;
//     }
// }

// void *productor(void *args)
// {
//     BlockQueue<int> *pbq = static_cast<BlockQueue<int> *>(args);

//     while (true)
//     {
//         sleep(1);
//         int num = rand() % 100;
//         pbq->push(num);
//         cout << "生产者生产了一个数据：" << num << endl;
//     }
// }

// int main()
// {
//     srand((unsigned long)time(nullptr) ^ getpid());
//     BlockQueue<int> bq;

//     //创建生产者、消费者线程
//     pthread_t c, p;
//     pthread_create(&c, nullptr, consumer, &bq);
//     pthread_create(&p, nullptr, productor, &bq);

//     pthread_join(c, nullptr);
//     pthread_join(p, nullptr);

//     return 0;
// }