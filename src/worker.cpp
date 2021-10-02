//
// Created by yao jun on 2021/10/1 22:59.
//
#include <iostream>
#include <unistd.h>
#include "worker.h"

using namespace std;

void Worker::operator()() {
    cout<<"working id: " << id <<endl;
//    while (true){
//        cout<<"working id: " << id <<endl;
//        usleep(100000); // 0.1s
//    }
}
