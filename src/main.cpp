#include <Core>
#include <iostream>
#include "cat.h"
#include "sunnet.h"
#include <cmath>
#include <mutex>
#include <unistd.h>
using namespace Eigen;
using namespace std;

void test()
{
    MatrixXd m = MatrixXd::Random(3,3);
    m = (m + MatrixXd::Constant(3,3,1.2)) * 50;
    cout << "m =" << endl << m << endl;
    VectorXd v(3);
    v << 1, 2, 3;
    cout << "m * v =" << endl << m * v << endl;
}

void test1(){
    Cat cat;
    cat.age = 2;
    cat.Say();
}
void hello(){
    cout<<"hello world"<<endl;
}
void world(string name){
    cout<<name<<endl;
}
void test2(){
    thread t(hello); // 创建一个thread对象，并指定入口函数。
    t.join();
    thread t2([]{
        cout<<"hello world lambda"<<endl;
    });
    t2.join();
    thread t3(world, "transfer parameter"); // 传递参数 拷贝传参
    t3.join();
}
void test3(){
    new Sunnet();
    Sunnet::inst->Start();
    Sunnet::inst->Wait();
}
static const int MAX = 10e8;
static double sum = 0;
void worker(int minV, int maxV){
    for(int i = minV; i <= maxV; i++){
        sum +=  sqrt(i);
    }
}
void serial_task(){
    // 单线程 计算1e8的平方根之和
    auto start_time = chrono::steady_clock::now();
    sum = 0;
    worker(1, MAX);
    auto end_time = chrono::steady_clock::now();
    auto ms = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();
    cout<<"Serial task finish, "<< ms <<" ms consumed, Result: " << sum << endl;


}
static mutex mtx;
void concurrent_worker(int minV, int maxV){
    double tmp_sum = 0;
    for(int i = minV; i <= maxV; i++){
        tmp_sum +=  sqrt(i);

    }
    mtx.lock();
    sum += tmp_sum;
    mtx.unlock();
}
void concurrent_task(int minV, int maxV){
    auto start_time = chrono::steady_clock::now();
    unsigned concurrent_count = thread::hardware_concurrency(); // 获取当前硬件支持多少个线程并发执行
    cout<< " hardware_concurrency: "<< concurrent_count<< endl;
    vector<thread> threads;
    minV = 0;
    sum = 0;
    for(int t = 0; t < concurrent_count; t++){
        int range = maxV / concurrent_count *(t + 1);
        threads.push_back(thread(concurrent_worker, minV, range));
        minV = range + 1;
    }
    for(auto &t: threads){
        t.join();
    }
    auto end_time = chrono::steady_clock::now();
    auto ms = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();
    cout<<"Serial task finish, "<< ms <<" ms consumed, Result: " << sum << endl;
}

void test_service(){
    auto pingType = make_shared<string>("ping");
    uint32_t ping1 = Sunnet::inst->NewService(pingType);
    uint32_t ping2 = Sunnet::inst->NewService(pingType);
    uint32_t pong = Sunnet::inst->NewService(pingType);

}
int test_thread(){
    serial_task();
    concurrent_task(1, MAX);
    return 0;
}
int main(){
    new Sunnet;
    Sunnet::inst->Start(); // 先启动系统，开启3个工作线程
    // 等你搞完行不行
    usleep(1000000);
    test_service(); // 创建服务
//    cout<<Sunnet::inst->services.size()<<endl;
    Sunnet::inst->Wait();
    return 0;
}