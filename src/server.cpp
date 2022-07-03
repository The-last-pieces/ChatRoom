#include "net/socket.hpp"
#include <iostream>
#include <thread>
#include <optional>

const char *ip      = "10.0.24.2";
const int   port    = 8889;
const int   backlog = 5;
const int   buf_len = 1024;

int tcp_main() {
    // 服务端创建socket
    Socket server(ip, port, SocketType::TCP);

    // 绑定地址信息
    server.bind_port();

    // 监听端口
    server.listen_port(backlog);

    // 等待客户端连接
    while (true) {
        // 从请求队列中获取一个连接
        Socket client = server.accept_client();
        std::thread([client = std::move(client)]() {
            std::string buf;
            std::string desc = client.get_desc();

            printf("client<%s> connected\n", desc.data());

            while (true) {
                // 接收客户端的数据
                int ret = client.recv_data(buf, buf_len);
                if (ret == 0) break;

                printf("recv %d bytes from client<%s> :\n[%s]\n", ret, desc.data(), buf.data());

                client.send_data("string size : " + std::to_string(ret));
            }

            printf("client<%s> quit\n", desc.data());
        }).detach();
    }
    return 0;
}

int udp_main() {
    Socket      server(ip, port, SocketType::UDP);
    sockaddr_in addr;
    std::string buf;

    server.bind_port();

    while (true) {
        int ret = server.recvfrom_data(buf, 1024, addr);

        printf("recv %d bytes from client<%s:%d>: \n[%s]\n", ret, inet_ntoa(addr.sin_addr), ntohs(addr.sin_port), buf.data());

        server.sendto_data("string size : " + std::to_string(ret), addr);
    }
}

int main() {
    udp_main();
}
