//
// Created by yao jun on 2021/10/1 22:58.
//

#ifndef MAT_WORKER_H
#define MAT_WORKER_H
#include <thread>


class Sunnet;
using namespace std;
class Worker{
public:
    int id; //编号
    int eachNum;  // 每次处理多少条消息
    void operator()(); //线程函数
};
#endif //MAT_WORKER_H
