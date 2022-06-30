#include "net/socket.hpp"
#include <iostream>

const char *ip   = "10.0.24.2";
const int   port = 8889;

int tcp_main() {
    // 构造客户端socket
    Socket client(ip, port);

    // 连接服务端
    client.connect_server();

    printf("client<%s:%d> start\n", client.get_ip().data(), client.get_port());

    // 发送数据
    while (true) {
        std::string buf;
        printf("input to send: ");
        std::getline(std::cin, buf);

        client.send_data(buf);

        client.recv_data(buf, 1024);
        printf("recv from server: '\n%s\n'\n", buf.data());
    }

    return 0;
}

int udp_main() {
    Socket      client(ip, port, SocketType::UDP);
    std::string buf;

    while (true) {
        printf("input to send: ");
        std::getline(std::cin, buf);

        client.sendto_data(buf);

        int len = client.recvfrom_data(buf, 1024);
        printf("recv %d from server: '\n%s\n'\n", len, buf.data());
    }
}

int main() {
    udp_main();
}
