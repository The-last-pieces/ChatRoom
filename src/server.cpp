#include "net/tcp/socket.hpp"
#include <iostream>
#include <thread>

int main() {
    const char *ip      = "10.0.24.2";
    const int   port    = 8889;
    const int   backlog = 5;

    // 服务端创建socket
    TcpSocket server(ip, port);

    // 监听端口
    server.bind_and_listen(backlog);

    // 等待客户端连接
    while (true) {
        // 从请求队列中获取一个连接
        TcpSocket client = server.accept_client();
        std::thread([client = std::move(client)]() {
            printf("client<%s:%d> connected\n", client.get_ip().data(), client.get_port());
            while (true) {
                // 接收客户端的数据
                std::string buf = client.recv_data(1024);
                if (buf.empty()) {
                    printf("client<%s:%d> quit\n", client.get_ip().data(), client.get_port());
                    break;
                }
                printf("recv from client<%s:%d>: '\n%s\n'\n", client.get_ip().data(), client.get_port(), buf.data());
                client.send_data("string size : " + std::to_string(buf.size()));
            }
        }).detach();
    }
    return 0;
}