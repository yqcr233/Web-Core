#include "event_loop_thread.hpp"
#include "event_loop.hpp"

EventLoopThread::EventLoopThread(const ThreadInitCallback &cb, const string& name):
                                    loop_(nullptr), exiting_(false), thread_(bind(&EventLoopThread::threadFunc, this), name),
                                    mutex_(), cond_(), callback_(cb){}
EventLoopThread::~EventLoopThread(){
    exiting_ = true;
    if(loop_ != nullptr) {
        loop_->quit();
        thread_.join();
    }
}
// 获取线程所绑定的loop循环返回到所在线程池进行绑定
EventLoop* EventLoopThread::startLoop(){
    thread_.start();
    
    EventLoop *loop = nullptr;
    {
        unique_lock<std::mutex> lock(mutex_);
        cond_.wait(lock, [this](){
            return loop_ != nullptr;
        });
        loop = loop_;
    }
    return loop;
}
void EventLoopThread::threadFunc(){
    EventLoop _loop;
    if(callback_) {
        callback_(&_loop);
    }
    {
        unique_lock<std::mutex> lock(mutex_);
        loop_ = &_loop;
        cond_.notify_one();
    }

    _loop.loop();
    unique_lock<std::mutex> lock(mutex_);
    loop_ = nullptr;
}