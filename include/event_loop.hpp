#ifndef EVENTLOOP_HPP
#define EVENTLOOP_HPP
#include "core.hpp"
#include "noncopyable.hpp"

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
        
};
#endif