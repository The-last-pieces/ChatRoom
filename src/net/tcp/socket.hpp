#ifndef M_SOCKET_H
#define M_SOCKET_H

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string_view>
#include "../../utils/tools.hpp"
/*
封装socket操作
*/

// Tcp使用套接字类
class TcpSocket {
private:
    int                fd = -1; // 套接字的文件描述符
    struct sockaddr_in addr {}; // 地址信息
public:
    std::string_view get_ip() const {
        return std::string_view(inet_ntoa(addr.sin_addr));
    }

    int get_port() const {
        return ntohs(addr.sin_port);
    }

private:
    TcpSocket(int fd, struct sockaddr_in addr) :
        fd(fd), addr(addr) {
    }

public:
    TcpSocket() = default;
    ~TcpSocket() {
        if (fd != -1)
            close_socket();
    }

    // 处理拷贝
    TcpSocket(const TcpSocket &) = delete;
    TcpSocket(TcpSocket &&rhs) noexcept :
        fd(rhs.fd), addr(rhs.addr) {
        rhs.fd = -1;
    }

    // 处理赋值
    TcpSocket &operator=(const TcpSocket &) = delete;
    TcpSocket &operator=(TcpSocket &&rhs) noexcept {
        if (this != &rhs) {
            fd     = rhs.fd;
            addr   = rhs.addr;
            rhs.fd = -1;
        }
        return *this;
    }

    // 任意端调用:构造socket
    TcpSocket(const char *ip, int port) {
        fd = socket(AF_INET, SOCK_STREAM, 0);
        ToolUtils::check_error(fd, "socket");
        addr.sin_family      = AF_INET;
        addr.sin_port        = htons(port);
        addr.sin_addr.s_addr = inet_addr(ip);
    }

    // 客户端调用:连接IP和port对应的服务端
    void connect_to_server() const {
        int ret = connect(fd, (struct sockaddr *)&addr, sizeof(addr));
        ToolUtils::check_error(ret, "connect");
    }

    // 服务端调用:绑定IP和port, 并监听客户端的连接
    void bind_and_listen(int backlog) const {
        int ret = bind(fd, (struct sockaddr *)&addr, sizeof(addr));
        ToolUtils::check_error(ret, "bind");
        ret = listen(fd, backlog);
        ToolUtils::check_error(ret, "listen");
    }

    // 服务端调用:接收客户端的连接
    TcpSocket accept_client() const {
        struct sockaddr_in caddr;
        socklen_t          len = sizeof(caddr);
        int                cfd = accept(fd, (struct sockaddr *)&caddr, &len);
        ToolUtils::check_error(cfd, "accept");
        return TcpSocket(cfd, caddr);
    }

    // 任意端调用:关闭套接字
    void close_socket() const {
        close(fd);
    }

    // 任意端调用:发送数据
    ssize_t send_data(std::string_view data) const {
        ssize_t ret = send(fd, data.data(), data.size(), 0);
        return ret;
    }

    // 任意端调用:接收数据
    std::string recv_data(int maxlen) const {
        std::string data(maxlen, '\0');
        int         ret = recv(fd, data.data(), maxlen, 0);
        data.resize(ret);
        return data;
    }
};

#endif