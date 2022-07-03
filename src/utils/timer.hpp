#ifndef M_TIMER_H
#define M_TIMER_H

#include <sys/timerfd.h>
#include <unordered_set>
#include <functional>
#include <mutex>
#include <queue>
#include "thread/locker.hpp"

/*
定时器模块 Todo
*/

class Timer {
    using Callable = std::function<void()>;

    // 包装节点
    struct TimerNode {
        int      id;       // 定时器id
        int      delay;    // 延迟的毫秒数
        bool     repeat;   // 是否重复
        bool     cancel;   // 是否取消了
        Callable callback; // 定时器回调函数

        TimerNode(int id, int delay, bool repeat, Callable &&callback) :
            id(id), delay(delay), repeat(repeat), callback(std::move(callback)), expire(clock() + delay) {
        }

        clock_t expire; // 到期时间戳

        // a<b则a的优先级在b之后,保证到期事件更早的任务在顶部
        friend bool operator<(const TimerNode &a, const TimerNode &b) {
            return a.expire > b.expire;
        }
    };

    std::priority_queue<TimerNode> heap;        // 优先队列
    std::unordered_set<int>        cancels;     // 取消的定时器id
    int                            counter = 0; // 定时器id计数器
    Mutex                          locker;      // 资源锁

public:
    // 定时器句柄
    class TimerHandler {
        friend class Timer;

        int id;

        TimerHandler(int id) :
            id(id) {}

    public:
        // 取消任务
        void cancel() {
            Timer::global.cancelTimer(id);
        }
    };

private:
    // 定时重复某个任务
    TimerHandler setInterval(Callable &&callback, int ms) {
        std::lock_guard<Mutex> lock(locker);

        int id = counter++;
        heap.emplace(id, ms, true, std::move(callback));

        return TimerHandler(id);
    }

    // 延迟执行一个任务,不重复
    TimerHandler setTimeout(Callable &&callback, int ms) {
        std::lock_guard<Mutex> lock(locker);

        int id = counter++;
        heap.emplace(id, ms, false, std::move(callback));

        return TimerHandler(id);
    }

private:
    // 取消一个定时器
    void cancelTimer(int id) {
        std::lock_guard<Mutex> lock(locker);
        cancels.emplace(id);
    }

    // 定时器主逻辑 Todo
    void timerLoop() {
        time_t cur = time(NULL);
        while (!heap.empty()) {
            TimerNode tmp = heap.top();
            if (tmp.expire > cur) break;
            try {
                tmp.callback();
            } catch (...) {
                printf("timer task error!\n");
            }
            heap.pop();
            if (tmp.repeat) {
                tmp.expire = cur + tmp.delay;
                heap.emplace(std::move(tmp));
            }
        }
    }

private:
    static Timer global; // 全局定时器

    Timer() {}

public:
    static TimerHandler setInterval(Callable &&callback, int ms) {
        return global.setInterval(std::move(callback), ms);
    }

    static TimerHandler setTimeout(Callable &&callback, int ms) {
        return global.setTimeout(std::move(callback), ms);
    }
};

inline Timer Timer::global;

#endif