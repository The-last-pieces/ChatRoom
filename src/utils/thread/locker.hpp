#ifndef M_LOCKER_H
#define M_LOCKER_H

#include <semaphore.h>
#include "../tools.hpp"

/*
线程流程控制模块
*/

// 信号量
class Semaphore {
private:
    sem_t m_sem;

public:
    Semaphore(int value = 0) {
        ToolUtils::error_when_not(sem_init(&m_sem, 0, value), "sem_init");
    }
    ~Semaphore() {
        ToolUtils::error_when_not(sem_destroy(&m_sem), "sem_destroy");
    }

public:
    void wait() {
        ToolUtils::error_when_not(sem_wait(&m_sem), "sem_wait");
    }
    void post() {
        ToolUtils::error_when_not(sem_post(&m_sem), "sem_post");
    }
};

// 互斥量
class Mutex {
    friend class Condition;

private:
    pthread_mutex_t m_mutex;

public:
    Mutex() {
        ToolUtils::error_when_not(pthread_mutex_init(&m_mutex, nullptr), "mutex_init");
    }

    ~Mutex() {
        ToolUtils::error_when_not(pthread_mutex_destroy(&m_mutex), "mutex_destroy");
    }

public:
    void lock() {
        ToolUtils::error_when_not(pthread_mutex_lock(&m_mutex), "mutex_lock");
    }

    bool try_lock() {
        return pthread_mutex_trylock(&m_mutex) == 0;
    }

    void unlock() {
        ToolUtils::error_when_not(pthread_mutex_unlock(&m_mutex), "mutex_unlock");
    }
};

// 条件变量
class Condition {
private:
    Mutex          m_mutex;
    pthread_cond_t m_cond;

public:
    Condition() {
        ToolUtils::error_when_not(pthread_cond_init(&m_cond, nullptr), "cond_init");
    }
    ~Condition() {
        ToolUtils::error_when_not(pthread_cond_destroy(&m_cond), "cond_destroy");
    }

public:
    void wait() {
        m_mutex.lock();
        ToolUtils::error_when_not(pthread_cond_wait(&m_cond, &m_mutex.m_mutex), "cond_wait");
        m_mutex.unlock();
    }

    void notify() {
        ToolUtils::error_when_not(pthread_cond_signal(&m_cond), "cond_signal");
    }

    void notify_all() {
        ToolUtils::error_when_not(pthread_cond_broadcast(&m_cond), "cond_broadcast");
    }
};

#endif