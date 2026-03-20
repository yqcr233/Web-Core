#ifndef EVENT_LOOP_THREAD_HPP
#define EVENT_LOOP_THREAD_HPP
#include "core.hpp"
#include "noncopyable.hpp"
#include "thread_.hpp"
class EventLoop;

class EventLoopThread: noncopyable{
    public:
        using ThreadInitCallback = function<void(EventLoop*)>;
        EventLoopThread(const ThreadInitCallback &cb = ThreadInitCallback(), const string& name = string());
        ~EventLoopThread();
        EventLoop* startLoop();
    private:
        void threadFunc();
    private:
        EventLoop *loop_;
        bool exiting_;
        Thread_ thread_;
        mutex mutex_;
        condition_variable cond_;
        ThreadInitCallback callback_;
};
#endif