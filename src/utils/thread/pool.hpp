#ifndef M_POOL_H
#define M_POOL_H

#include <functional>
/*
线程池模块 Todo
*/

class ThreadPool {
    using Callable = std::function<void()>;

public:
    ThreadPool(size_t thread_number = 8, size_t max_tasks = 10000);
    ~ThreadPool();

public:
    void add_task(Callable &&task);
};

#endif