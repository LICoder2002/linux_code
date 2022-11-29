#include "ThreadPool.hpp"
#include "Task.hpp"

int main()
{
    srand((unsigned long)time(nullptr));
    const string ops = "+-*/%";
    // unique_ptr<ThreadPool<Task>> tp(new ThreadPool<Task>());
    unique_ptr<ThreadPool<Task>> tp(ThreadPool<Task>::getInstance());

    tp->start();

    while (true)
    {

        int elem1 = rand() % 50;
        int elem2 = rand() % 50;
        char oper = ops[rand() % ops.size()];

        Task t(elem1, elem2, oper);

        Log() << "主线程派发了一个任务：" << elem1 << " " << oper << " " << elem2 << " = ? " << endl;

        // tp->push(Task(elem1, elem2, oper));
        tp->push(t);
        sleep(1);
    }

    return 0;
}