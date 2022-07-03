#ifndef M_SOCKET_H
#define M_SOCKET_H

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string_view>
#include "../utils/tools.hpp"

/*
封装socket操作
*/

enum class SocketType {
    TCP,
    UDP
};

// 套接字类,支持tcp/udp
class Socket {
private:
    int                fd = -1; // 套接字的文件描述符
    struct sockaddr_in addr {}; // 地址信息
public:
    std::string get_ip() const {
        return inet_ntoa(addr.sin_addr);
    }

    int get_port() const {
        return ntohs(addr.sin_port);
    }

    int get_fd() const {
        return fd;
    }

    std::string get_desc() const {
        return inet_ntoa(addr.sin_addr) + std::string(":") + std::to_string(get_port());
    }

private:
    Socket(int fd, struct sockaddr_in addr) :
        fd(fd), addr(addr) {
    }

public:
    Socket() = default;
    ~Socket() {
        if (fd != -1)
            close_socket();
    }

    // 处理拷贝
    Socket(const Socket &) = delete;
    Socket(Socket &&rhs) noexcept :
        fd(rhs.fd), addr(rhs.addr) {
        rhs.fd = -1;
    }

    // 处理赋值
    Socket &operator=(const Socket &) = delete;
    Socket &operator=(Socket &&rhs) noexcept {
        if (this != &rhs) {
            fd     = rhs.fd;
            addr   = rhs.addr;
            rhs.fd = -1;
        }
        return *this;
    }

    // 任意端调用:构造socket
    Socket(const char *ip, int port, SocketType type = SocketType::TCP) {
        const int proto = type == SocketType::TCP ? SOCK_STREAM : SOCK_DGRAM;

        fd = socket(AF_INET, proto, 0);
        ToolUtils::error_when_is(fd, "socket");
        addr.sin_family      = AF_INET;
        addr.sin_port        = htons(port);
        addr.sin_addr.s_addr = inet_addr(ip);
    }

    // tcp/udp客户端调用:连接IP和port对应的服务端
    void connect_server() const {
        int ret = connect(fd, (struct sockaddr *)&addr, sizeof(addr));
        ToolUtils::error_when_is(ret, "connect");
    }

    // tcp/udp服务端调用:绑定端口
    void bind_port() const {
        int ret = bind(fd, (struct sockaddr *)&addr, sizeof(addr));
        ToolUtils::error_when_is(ret, "bind");
    }

    // tcp服务端调用:监听端口
    void listen_port(int backlog) const {
        int ret = listen(fd, backlog);
        ToolUtils::error_when_is(ret, "listen");
    }

    // tcp服务端调用:接收客户端的连接
    Socket accept_client() const {
        struct sockaddr_in caddr;

        socklen_t len = sizeof(caddr);
        int       cfd = accept(fd, (struct sockaddr *)&caddr, &len);
        ToolUtils::error_when_is(cfd, "accept");

        return Socket(cfd, caddr);
    }

    // 任意端调用:关闭套接字
    void close_socket() const {
        close(fd);
    }

public:
    // tcp任意端调用:发送数据,返回成功发送的字节数.出错则返回-1
    int send_data(std::string_view data) const {
        int ret = send(fd, data.data(), data.size(), 0);
        return ret;
    }

    // tcp任意端调用:在字符串上接收数据,返回成功接收的字节数
    int recv_data(std::string &buf, int maxlen) const {
        buf.resize(maxlen);
        int ret = recv(fd, buf.data(), maxlen - 1, 0);
        if (ret >= 0) {
            buf.resize(ret), buf[ret] = 0;
        }
        return ret;
    }

    // tcp任意端调用:在原始指针上接收数据,返回成功接收的字节数.出错则返回-1
    int recv_data(char *buf, int maxlen) {
        int ret = recv(fd, buf, maxlen - 1, 0);
        if (ret >= 0) {
            buf[ret] = 0;
        }
        return ret;
    }

public:
    // udp服务端调用:发送数据,返回成功发送的字节数.出错则返回-1,必须指定目的地址
    int sendto_data(std::string_view data, const struct sockaddr_in &addr) const {
        int ret = sendto(fd, data.data(), data.size(), 0, (struct sockaddr *)&addr, sizeof(addr));
        return ret;
    }

    // udp客户端调用:发送数据,返回成功发送的字节数.出错则返回-1.默认发送到服务端
    int sendto_data(std::string_view data) const {
        return sendto_data(data, addr);
    }

    // udp服务端调用:接收数据,返回成功接收的字节数.出错则返回-1
    int recvfrom_data(std::string &buf, int maxlen, struct sockaddr_in &addr) const {
        buf.resize(maxlen);
        socklen_t len = sizeof(addr);
        int       ret = recvfrom(fd, buf.data(), maxlen - 1, 0, (struct sockaddr *)&addr, &len);
        if (ret >= 0) {
            buf.resize(ret), buf[ret] = 0;
        }
        return ret;
    }

    // udp客户端调用:接收数据,返回成功接收的字节数.出错则返回-1
    int recvfrom_data(std::string &buf, int maxlen) const {
        buf.resize(maxlen);
        socklen_t len = sizeof(addr);
        int       ret = recvfrom(fd, buf.data(), maxlen - 1, 0, NULL, 0);
        if (ret >= 0) {
            buf.resize(ret), buf[ret] = 0;
        }
        return ret;
    }

    // udp服务端调用:接收数据,返回成功接收的字节数.出错则返回-1. 并返回目的地址
    int recvfrom_data(char *buf, int maxlen, struct sockaddr_in &addr) {
        socklen_t len = sizeof(addr);
        int       ret = recvfrom(fd, buf, maxlen - 1, 0, (struct sockaddr *)&addr, &len);
        if (ret >= 0) {
            buf[ret] = 0;
        }
        return ret;
    }

    // udp客户端调用:接收数据,返回成功接收的字节数.出错则返回-1.不需要知道地址
    int recvfrom_data(char *buf, int maxlen) {
        socklen_t len = sizeof(addr);
        int       ret = recvfrom(fd, buf, maxlen - 1, 0, NULL, 0);
        if (ret >= 0) {
            buf[ret] = 0;
        }
        return ret;
    }
};

#endif