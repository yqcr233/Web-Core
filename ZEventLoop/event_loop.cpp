#include "event_loop.hpp"
#include <sys/eventfd.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include "logger.hpp"
#include "channel.hpp"
#include "poller.hpp"

// 防止线程创建多个eventloop
__thread EventLoop *t_loopInThisThread = nullptr;

// poller超时时间
const int KPollTimeMs = 10000;  // 10000ms = 10s

/**
 * 使用eventfd在线程之间进行通信无需上锁进行同步
 */
int createEventfd(){
    int evtfd = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC); // 创建eventfd，设置事件计数器初始为0
    if(evtfd < 0) {
        LOG_FATAL("eventfd create error:%d\n", errno);
    }
    return evtfd;
}
 
EventLoop::EventLoop(): 
            looping_(false), quit_(false), callingPendingFunctors_(false), 
            threadId_(tid()), poller_(Poller::newDefaultPoller(this)), wakeupFd_(createEventfd()),
            wakeupChannel_(new Channel{this, wakeupFd_})
{
    LOG_INFO("EventLoop created %p in thread %d\n", this, threadId_);
    if(t_loopInThisThread) {
        LOG_FATAL("Another EventLoop %p exists in this thread %d\n", t_loopInThisThread, threadId_);
    } else {
        t_loopInThisThread = this;
    }
    wakeupChannel_->setReadCallback(bind(&EventLoop::handleRead, this));
    wakeupChannel_->enableReading(); // 监听wakeChannel_的EPOLL读事件
}

EventLoop::~EventLoop(){
    wakeupChannel_->disableAll();
    wakeupChannel_->remove();
    close(wakeupFd_);
    t_loopInThisThread = nullptr;
}

void EventLoop::loop(){// 开启循环
    looping_ = true;
    quit_ = false;
    LOG_INFO("EventLoop %p start looping\n", this);

    while(!quit_) {
        activeChannels_.clear();
        pollReturnTime_ = poller_->poll(KPollTimeMs, &activeChannels_);
        for(Channel* channel: activeChannels_) {
            channel->handleEvent(pollReturnTime_);
        }
        // 执行当前Eventloop事件循环中需要处理的回调操作
        doPendingFunctors();
    }

    LOG_INFO("EventLoop %p stop looping.\n", this);
    looping_ = false;
}

void EventLoop::quit(){// 退出循环
    quit_ = true;
    // 如果是其他线程loop中quit当前线程loop则需先唤醒当前loop执行完剩余poll中事件
    if(!isInLoopThread()) {
        wakeup();
    }
}

TimeStamp EventLoop::pollReturnTime() const{
    return pollReturnTime_;
}

void EventLoop::runInLoop(function<void()> cb){
    if(isInLoopThread()) {
        cb();
    } else{
        queueInLLoop(cb);
    }
}

void EventLoop::queueInLLoop(function<void()> cb){
    {
        unique_lock<std::mutex> lock(mutex);
        pendingFunctors_.emplace_back(cb);
    }

    // !isInLoopThread => 在其他线程中调用，当前线程需先被唤醒确保不会出现poll阻塞的状态
    //  callingPendingFunctors_ => 当前线程并为阻塞，但当前循环poll调用已过，正在执行funcotr，所以提前唤醒防止下个循环中poll阻塞导致延迟
    if(!isInLoopThread() || callingPendingFunctors_) {
        wakeup();
    }

}

// 唤醒loop所在线程，向wakefd写入一个数据，wakeupChannel就发生读事件 当前loop线程就会被唤醒
void EventLoop::wakeup(){
    uint64_t one = 1;
    ssize_t n = write(wakeupFd_, &one, sizeof(one));
    if(n != sizeof(one)) {
        LOG_ERROR("EventLoop::wakeup() writes %lu bytes instead of 8\n", n);
    }
}

void EventLoop::updateChannel(Channel *channel){
    poller_->updateChannel(channel);
}

void EventLoop::removeChannel(Channel *channel){
    poller_->removeChannel(channel);
}

void EventLoop::hasChannel(Channel *channel){
    poller_->hasChannel(channel);
}

bool EventLoop::isInLoopThread() const{// 判断该loop对象是否是在其创建线程中执行
    return threadId_ == tid();
}

void EventLoop::handleRead(){
    uint64_t one = 1;
    ssize_t n = read(wakeupFd_, &one, sizeof(one));
    if(n != sizeof(one)) {
        LOG_ERROR("EventLoop::handleRead() reads %lu bytes instead of 8\n", n);
    }
}

void EventLoop::doPendingFunctors(){// 执行上层回调
    vector<function<void()>> functors;
    callingPendingFunctors_ = true;

    {
        unique_lock<std::mutex> lock(mutex);
        functors.swap(pendingFunctors_);
    }
    for(const function<void()> &functor: functors){
        functor();
    }
    
    callingPendingFunctors_ = false;
}

