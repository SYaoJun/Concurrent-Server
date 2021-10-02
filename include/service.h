//
// Created by yao jun on 2021/10/2 12:45.
//

#ifndef MAT_SERVICE_H
#define MAT_SERVICE_H
#include <queue>
#include <thread>
#include "msg.h"
using namespace std;

class Service{
public:
    uint32_t id;
    shared_ptr<string> type;
    bool isExiting = false;
    // 消息列表和锁
    queue<shared_ptr<BaseMsg>> msgQueue;
    pthread_spinlock_t queueLock;
    Service();
    ~Service();
    // 回调函数
    void OnInit();
    void OnMsg(shared_ptr<BaseMsg> msg);
    void OnExit();
    // 插入消息
    void PushMsg(shared_ptr<BaseMsg> msg);
    // 执行消息
    bool ProcessMsg();
    void ProcessMsgs(int max);
private:
    shared_ptr<BaseMsg> PopMsg();
};
#endif //MAT_SERVICE_H
