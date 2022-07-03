#ifndef M_ROOM_H
#define M_ROOM_H

#include <set>
#include <list>
#include "user.hpp"
#include "message.hpp"
#include "../utils/tools.hpp"

/*
封装聊天室api

流程:
客户端发消息:
    1. 创建一个新的消息
    2. 发送消息给服务器
    3. 服务端保存消息到数据库
    4. 服务端将消息转发给所有在聊天室中的用户
客户端上线时,请求服务器的最新消息.
客户端定时请求最新消息,防止遗漏.
*/
class RoomClient {
private:
    int               m_roomId;     // 聊天室id
    std::set<User>    m_users;      // 一个房间内的所有用户,按用户id排序
    std::set<Message> m_messages;   // 一个房间内的所有聊天记录,按发送时间排序
    User              m_self;       // 客户端登陆的用户
    int               m_messageIdx; // 顶部消息的下标
public:
    RoomClient(int roomId, User self) :
        m_roomId(roomId), m_self(self) {
        fetchUsers();
        fetchMessages();
        setScrollPos(m_messages.size());
    }

    // 在控制台渲染聊天室信息 Todo
    void repaint() {}

    // 设置滚轮位置, pos取值为[1,n]
    void setScrollPos(int pos) {
        if (m_messages.empty()) {
            m_messageIdx = 0;
        } else {
            m_messageIdx = ToolUtils::clamp(1, pos, getMessageCount());
        }
    }

    // 获取滚轮位置
    int getScrollPos() {
        return m_messageIdx;
    }

    // 消息个数
    int getMessageCount() {
        return m_messages.size();
    }

    // 发送消息
    void sendMessage(const Message &msg) {
        addMessage(msg);

        // 广播给其他用户 Todo
    }

private:
    // 请求最新的用户信息 Todo
    void fetchUsers() {}

    // 请求最新的聊天记录 Todo
    void fetchMessages() {}

    // 添加消息到渲染集合中
    void addMessage(const Message &msg) {
        m_messages.insert(msg);

        // 在数据库中添加 Todo

        repaint();
    }
};

#endif