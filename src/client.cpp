#include "net/tcp/socket.hpp"
#include <iostream>

int main() {
    const char *ip   = "10.0.24.2";
    const int   port = 8889;

    // 构造客户端socket
    TcpSocket client(ip, port);

    // 连接服务端
    client.connect_to_server();

    printf("client<%s:%d> start\n", client.get_ip().data(), client.get_port());

    // 发送数据
    while (true) {
        std::string buf;
        printf("input to send: ");
        std::getline(std::cin, buf);

        client.send_data(buf);

        buf = client.recv_data(1024);
        printf("recv from server: '\n%s\n'\n", buf.data());
    }

    return 0;
}