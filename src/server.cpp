#include <cstdio>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

// using namespace std;

#define ERROR_CHECK(ret, msg) \
  if (ret == -1) {            \
    perror(msg);              \
    exit(-1);                 \
  }

int main() {
  const char* ip = "10.0.24.2";
  const int port = 8889;
  const int backlog = 5;

  // 服务端创建socket
  int sfd = socket(AF_INET, SOCK_STREAM, 0);
  ERROR_CHECK(sfd, "socket");

  // 绑定指定ip和port
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = inet_addr(ip);
  int ret = bind(sfd, (struct sockaddr*)&addr, sizeof(addr));
  ERROR_CHECK(ret, "bind");

  // 监听客户端连接
  ret = listen(sfd, backlog);
  ERROR_CHECK(ret, "listen");

  // 等待客户端连接
  while (true) {
    // 从请求队列中获取一个连接
    struct sockaddr_in c_addr;
    socklen_t len = sizeof(c_addr);
    int cfd = accept(sfd, (struct sockaddr*)&c_addr, &len);
    ERROR_CHECK(cfd, "accept");

    while (true) {
      // echo服务器
      char buf[1024] = {0};
      ret = read(cfd, buf, sizeof(buf));
      ERROR_CHECK(ret, "read");
      if (ret == 0) {
        printf("client quit\n");
        break;
      }
      printf("recv: %s\n", buf);
      ret = write(cfd, buf, strlen(buf));
      ERROR_CHECK(ret, "write");
    }
  }
  return 0;
}