#ifndef M_TOOLS_H
#define M_TOOLS_H

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stdexcept>
#include <errno.h>

/*
封装Linux常用的操作
*/

class ToolUtils {
private:
    static void throw_error(const char *msg) {
        perror(msg);
        throw std::runtime_error(msg);
    }

public:
    // 检查Linux api的返回值 , 如果是目标则error
    static void error_when_is(int ret, const char *msg, int check = -1) {
        if (ret == check) throw_error(msg);
    }

    // 检查Linux api的返回值 , 如果不是目标则error
    static void error_when_not(int ret, const char *msg, int check = 0) {
        if (ret != check) throw_error(msg);
    }

    // 填充0
    template <typename T>
    static void bzero(T &ptr) {
        memset(&ptr, 0, sizeof(T));
    }
};

#endif