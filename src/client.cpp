#include "net/socket.hpp"
#include <iostream>

const char *ip      = "10.0.24.2";
const int   port    = 8889;
const int   buf_len = 1024;

int tcp_main() {
    // 构造客户端socket
    Socket      client(ip, port, SocketType::TCP);
    std::string buf;

    // 连接服务端
    client.connect_server();

    printf("client<%s> start\n", client.get_desc().data());

    // 发送数据
    while (true) {
        printf("input to send: ");
        std::getline(std::cin, buf);

        client.send_data(buf);

        int len = client.recv_data(buf, buf_len);
        printf("recv %d bytes from server: \n[%s]\n", len, buf.data());
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

        int len = client.recvfrom_data(buf, buf_len);
        printf("recv %d bytes from server: \n[%s]\n", len, buf.data());
    }
}

int main() {
    udp_main();
}
