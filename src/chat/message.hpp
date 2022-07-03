#ifndef M_MESSAGE_H
#define M_MESSAGE_H

#include <string>
/*
一条聊天记录
*/
class Message {
public:
    enum class Type {
        TEXT, // 纯文本
        FILE, // 文件
    };

private:
    const clock_t m_sendTime; // 发送时间戳
    const int     m_userId;   // 发送者id
    const int     m_roomId;   // 聊天室id
    const int     m_msgId;    // 消息id
    std::string   m_content;  // 消息内容
    Type          m_type;     // 消息类型
};

#endif