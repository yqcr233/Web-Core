#include "event_loop.hpp"


EventLoop::EventLoop(): 
            looping_(false), quit_(false), callingPendingFunctors_(false), 
            threadId_(tid())
{
    
}

EventLoop::~EventLoop(){

}
void EventLoop::loop(){// 开启循环

}
void EventLoop::quit(){// 退出循环

}

TimeStamp EventLoop::pollReturnTime() const{

}
void EventLoop::runInLoop(function<void()> cb){

}
void EventLoop::queueInLLoop(function<void()> cb){

}
void EventLoop::weakup(){

}

void EventLoop::updateChannel(Channel *channel){

}
void EventLoop::removeChannel(Channel *channel){

}
void EventLoop::hasChannel(Channel *channel){

}
bool EventLoop::isInLoopThread() const{// 判断该loop对象是否在当前线程中

}

void EventLoop::handleRead(){

}
void EventLoop::doPendingFunctors(){// 执行上层回调

}

