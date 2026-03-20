#ifndef EVENTLOOP_THREAD_POOL_HPP
#define EVENTLOOP_THREAD_POOL_HPP
#include "noncopyable.hpp"
#include "core.hpp"

class EventLoop;
class EventLoopThread;

class EventLoopTreadPool: noncopyable{
    public:
        using ThreadInitCallback = function<void(EventLoop*)>;
        EventLoopTreadPool(EventLoop *baseLoop, const string &nameArg);
        ~EventLoopTreadPool();

        void setThreadNum(int numThread) { _numThread = numThread;}
        void start(const ThreadInitCallback &cb);

        EventLoop* getNextLoop();
        vector<EventLoop*> getAllLoops();
        
        // 获取开始标志和线程池名字
        bool started() const {return _started;}
        const string name() const {return _name;}

    private:
        EventLoop* _baseLoop; // 用户直接创建的loop
        string _name; // 线程池名称
        bool _started; // 线程池启动标志
        int _numThread; // 线程池线程数量
        int _next; // 下一个链接准备的loop索引
        vector<unique_ptr<EventLoopThread>> _threads; // 线程列表
        vector<EventLoop*> _loops; // 每个线程的loop列表
} ;
#endif