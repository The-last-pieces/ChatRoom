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

  // 客户端请求连接
  int cfd = socket(AF_INET, SOCK_STREAM, 0);
  ERROR_CHECK(cfd, "socket");
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = inet_addr(ip);

  int ret = connect(cfd, (struct sockaddr*)&addr, sizeof(addr));
  ERROR_CHECK(ret, "connect");
  // 发送数据
  while (true) {
    char buf[1024] = {0};
    printf("send: ");
    scanf("%s", buf);
    ret = write(cfd, buf, strlen(buf));
    ERROR_CHECK(ret, "write");
    // 接收数据
    ret = read(cfd, buf, sizeof(buf));
    ERROR_CHECK(ret, "read");
    printf("recv: %s\n", buf);
  }

  return 0;
}