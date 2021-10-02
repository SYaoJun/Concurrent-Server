//
// Created by yao jun on 2021/10/2 12:35.
//

#ifndef MAT_MSG_H
#define MAT_MSG_H
#include <memory>
class BaseMsg{
public:
    enum TYPE{
        SERVICE = 1,
    };
    uint8_t type;
    char load[999999];
    virtual ~BaseMsg();
};

class ServiceMsg: public BaseMsg{
public:
    uint32_t source; // 消息发送方
    shared_ptr<char> buff; // 消息内容
    size_t size; // 消息内容大小
};
#endif //MAT_MSG_H
