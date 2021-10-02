//
// Created by yao jun on 2021/10/1 22:50.
//
#include <iostream>
#include "sunnet.h"

using namespace std;

// 很关键
Sunnet *Sunnet::inst;
Sunnet::Sunnet() {
    inst = this;
}
// 开启系统
void Sunnet::Start(){
    cout<<"Hello Sunnet"<<endl;
    pthread_rwlock_init(&serviceLock, nullptr);
    // 开启worker
    StartWorker();
}

void Sunnet::StartWorker() {
    for(int i = 0; i < WORKER_NUM; i++){
        cout << "start worker thread: " << i << endl;
        // 创建线程对象
        Worker* worker = new Worker();
        worker->id = i;
        worker->eachNum = 2 << i;
        // 创建线程
        thread* wt = new thread(*worker);
        // 添加进数组
        workers.push_back(worker);
        workerThreads.push_back(wt);

    }
}

void Sunnet::Wait() {
    if(workerThreads[0]){
        workerThreads[0]->join();
    }
}
/**
 * 新建服务
 * @param type
 * @return
 */
uint32_t Sunnet::NewService(shared_ptr<string> type) {
    auto srv = make_shared<Service>();
    srv->type = type;
    pthread_rwlock_wrlock(&serviceLock);
    {
        srv->id = maxId;
        maxId++;
        services.emplace(srv->id, srv);

    }
    pthread_rwlock_unlock(&serviceLock);
    srv->OnInit();
    return srv->id;
}

shared_ptr<Service> Sunnet::GetService(uint32_t id) {
    shared_ptr<Service> srv = nullptr;
    pthread_rwlock_rdlock(&serviceLock);
    {
        unordered_map<uint32_t, shared_ptr<Service>> :: iterator  iter = services.find(id);
        if(iter != services.end()){
            srv = iter->second;
        }
    }
    pthread_rwlock_unlock(&serviceLock);
    return srv;
}
void Sunnet::KillService(uint32_t id) {
    shared_ptr<Service> srv = GetService(id);
    if(!srv){
        return;
    }
    srv->OnExit();
    srv->isExiting = true;
    // 删除列表
    pthread_rwlock_wrlock(&serviceLock);
    services.erase(id);
    pthread_rwlock_unlock(&serviceLock);
}