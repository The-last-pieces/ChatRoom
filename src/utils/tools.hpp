#ifndef M_TOOLS_H
#define M_TOOLS_H

#include <cstdio>
#include <cstdlib>
#include <stdexcept>
#include <errno.h>

class ToolUtils {
public:
    // 检查Linux api的返回值 , 如果为-1则打印错误信息并抛出异常
    static void check_error(int ret, const char *msg) {
        if (ret == -1) {
            perror(msg);
            throw std::runtime_error(msg);
        }
    }
};

#endif