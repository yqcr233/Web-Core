#include "thread_.hpp"
#include "current_thread.hpp"

#include "semaphore.h"

std::atomic_int Thread_::numCreated_(0);

Thread_::Thread_(ThreadFunc func, const string &name): 
                    started_(false), joined_(false), tid_(0),
                    func_(move(func)), name_(name)                               
{
    setDefaultName();
}
Thread_::~Thread_(){
    if(started_ && !joined_) thread_->detach();
}
void Thread_::start(){
    started_ = true;
    sem_t sem;
    sem_init(&sem, false, 0);
    // 开启线程
    thread_ = shared_ptr<thread>(new thread([&](){
        tid_ = tid();
        sem_post(&sem);
        func_();
    }));
    // 等待确保获取新建线程的tid
    sem_wait(&sem);
}
void Thread_::join(){
    joined_ = true;
    thread_->join();
}
void Thread_::setDefaultName(){
    int num = ++numCreated_;
    if(name_.empty()) {
        char buf[32] = {0};
        snprintf(buf, sizeof(buf), "Thread%d", num);
        name_ = buf;
    }
}
// getter & setter
bool Thread_::started(){
    return started_;
}
pid_t Thread_::tid() const{
    return tid_;
}
const string& Thread_::name() const{
    return name_;
}
int Thread_::numCreated(){
    return numCreated_;
}
