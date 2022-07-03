#ifndef M_EPOLL_H
#define M_EPOLL_H

#include <sys/epoll.h>
#include <fcntl.h>
#include <unistd.h>
#include <functional>
#include "../utils/tools.hpp"

/*
封装epoll相关的操作
*/

class EpollManager {
private:
    // 最大事件个数
    static constexpr int MAX_EVENTS = 8096;

    // 事件表的文件描述符
    int m_epoll_fd = -1;

public:
    EpollManager() {
        m_epoll_fd = epoll_create(MAX_EVENTS);
        ToolUtils::error_when_is(m_epoll_fd, "epoll_create");
    }

    ~EpollManager() {
        if (m_epoll_fd != -1)
            close(m_epoll_fd);
    }

public:
    // 添加监听可读事件 , once表示是否一次性
    void listen_fd(int fd, bool once) {
        epoll_event ev;
        // 携带非阻塞的fd
        set_nonblock(fd);
        ev.data.fd = fd;
        // 读事件,ET模式,检测端口
        ev.events = EPOLLIN | EPOLLET | EPOLLRDHUP;
        // 是否一次性
        if (once) ev.events |= EPOLLONESHOT;
        epoll_ctl(m_epoll_fd, EPOLL_CTL_ADD, fd, &ev);
    }

    // 移除某个监听事件
    void remove_listen(int fd) {
        epoll_ctl(m_epoll_fd, EPOLL_CTL_DEL, fd, nullptr);
    }

    // 编辑并重置监听事件,会设置为一次性事件
    void edit_listen(int fd, uint32_t events) {
        epoll_event ev;
        ev.data.fd = fd;
        ev.events  = events | EPOLLET | EPOLLRDHUP | EPOLLONESHOT;
        epoll_ctl(m_epoll_fd, EPOLL_CTL_MOD, fd, &ev);
    }

    // 重置注册过的一次性事件
    void reset_listen(int fd) {
        epoll_event ev;
        ev.data.fd = fd;
        ev.events  = EPOLLIN | EPOLLET | EPOLLRDHUP | EPOLLONESHOT;
        epoll_ctl(m_epoll_fd, EPOLL_CTL_MOD, fd, &ev);
    }

public:
    // 阻塞直到出现事件, 并调用回调函数. 线程不安全
    template <typename Func>
    void wait_events(Func &&callback) {
        static epoll_event events[MAX_EVENTS];
        int                nfds = epoll_wait(m_epoll_fd, events, MAX_EVENTS, -1);
        ToolUtils::error_when_is(nfds, "epoll_wait");
        for (int i = 0; i < nfds; ++i) {
            callback(events[i]);
        }
    }

private:
    // 设置非阻塞属性
    static int set_nonblock(int fd) {
        int old = fcntl(fd, F_GETFL);
        fcntl(fd, F_SETFL, old | O_NONBLOCK);
        return old;
    }
};

#endif