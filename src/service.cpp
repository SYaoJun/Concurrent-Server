//
// Created by yao jun on 2021/10/2 14:54.
//
#include "sunnet.h"
#include "service.h"
#include <iostream>
Service::Service() {
    // 初始化锁
    pthread_spin_init(&queueLock, PTHREAD_PROCESS_PRIVATE);
}

Service::~Service() {
    pthread_spin_destroy(&queueLock);
}

void Service::PushMsg(shared_ptr<BaseMsg> msg) {
    pthread_spin_lock(&queueLock);
    msgQueue.push(msg);
    pthread_spin_unlock(&queueLock);
}

shared_ptr<BaseMsg> Service::PopMsg() {
    shared_ptr<BaseMsg> msg = nullptr;
    pthread_spin_lock(&queueLock);
    if(!msgQueue.empty()){
        msg = msgQueue.front();
        msgQueue.pop();
    }
    pthread_spin_unlock(&queueLock);
    return msg;
}

void Service::OnInit() {
    cout<<"["<< id <<"] OnInit success "<<endl;
}

void Service::OnMsg(shared_ptr<BaseMsg> msg) {
    cout<<"["<< id <<"] OnMsg"<<endl;
}

void Service::OnExit() {
    cout<<"["<<id<<"] OnExit"<<endl;
}

bool Service::ProcessMsg() {
    shared_ptr<BaseMsg> msg = PopMsg();
    if(msg){
        OnMsg(msg);
        return true;
    }else{
        return false;
    }
}

void Service::ProcessMsgs(int max) {
    for(int i = 0; i < max; i++){
        bool succ = ProcessMsg();
        if(!succ) break;
    }
}