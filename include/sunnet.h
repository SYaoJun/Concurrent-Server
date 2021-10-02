//
// Created by yao jun on 2021/10/1 22:49.
//

#ifndef MAT_SUNNET_H
#define MAT_SUNNET_H
#include "worker.h"
#include "service.h"
#include <vector>
#include <unordered_map>

class Sunnet{
public:
    static Sunnet* inst;
    Sunnet();
    void Start();
    void Wait();
    // 服务列表
    unordered_map<uint32_t, shared_ptr<Service>> services;
    uint32_t maxId = 0; // 最大ID
    pthread_rwlock_t serviceLock; // 读写锁
    // 增删服务
    uint32_t NewService(shared_ptr<string> type);
    void KillService(uint32_t id);
private:
    shared_ptr<Service> GetService(uint32_t id);
    int WORKER_NUM = 3;
    vector<Worker*> workers;
    vector<thread*> workerThreads;
    void StartWorker();
};
#endif //MAT_SUNNET_H
