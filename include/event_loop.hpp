#ifndef EVENTLOOP_HPP
#define EVENTLOOP_HPP
#include "core.hpp"
#include "noncopyable.hpp"
#include "timestamp.hpp"
#include "current_thread.hpp"

class Channel;
class Poller;

class EventLoop: noncopyable{
    public:
        EventLoop();
        ~EventLoop();
        void loop(); // 开启循环
        void quit(); // 退出循环
        
        TimeStamp pollReturnTime() const;
        void runInLoop(function<void()> cb);
        void queueInLLoop(function<void()> cb);
        void weakup();

        void updateChannel(Channel *channel);
        void removeChannel(Channel *channel);
        void hasChannel(Channel *channel);
        bool isInLoopThread() const; // 判断该loop对象是否在当前线程中
    private:    
        atomic_bool looping_, quit_;
        const pid_t threadId_; // 标记当前eventloop所属thread
        TimeStamp pollReturnTime_; // Poller返回发生事件channel的时间点
        unique_ptr<Poller> poller_;
        
        int wakeupFd_; // mainloop用来唤醒一个subloop处理新到的channel
        unique_ptr<Channel> wakeupChannel_;

        vector<Channel*> activeChannels_; // Poller检测到当前有事件发生的所有Channel列表
        atomic_bool callingPendingFunctors_; // 表示当前loop是否有需要执行的回调 
        vector<function<void()>> pendingFunctors_; // 存储loop需要执行的回调
        std::mutex mutex; // 管理上面vector的线程安全
    private:
        void handleRead();
        void doPendingFunctors(); // 执行上层回调
};
#endif