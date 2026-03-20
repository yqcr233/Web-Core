#include <memory>

#include "event_loop_thread_pool.hpp"
#include "event_loop_thread.hpp"
#include "logger.hpp"

EventLoopTreadPool::EventLoopTreadPool(EventLoop* baseLoop, const string &nameArg):
        _baseLoop(baseLoop), _name(nameArg), _started(false), _numThread(0), _next(0){}

EventLoopTreadPool::~EventLoopTreadPool(){}

void EventLoopTreadPool::start(const ThreadInitCallback& cb){
    _started = true;

    for(int i=0;i<_numThread;i++) {
        char buf[_name.size() + 32];
        snprintf(buf, sizeof buf, "%s%d", _name.c_str(), i); // 以“name+i”的格式创建thread名字
        EventLoopThread *t = new EventLoopThread(cb, buf);
        _threads.push_back(unique_ptr<EventLoopThread>(t));
        _loops.push_back(t->startLoop());
    }

    // 如果线程池无线程，则用当前线程执行baseloop
    if(_numThread == 0 && cb){
        cb(_baseLoop);
    }
}

// 以轮询的方式分配channel给subloop 
EventLoop* EventLoopTreadPool::getNextLoop(){
    EventLoop* loop = _baseLoop; // 默认使用baseloop

    if(!_loops.empty()) {
        loop = _loops[_next];
        _next++;
        if(_next >= _loops.size()) _next = 0;
    }

    return loop;
}

vector<EventLoop*> EventLoopTreadPool::getAllLoops(){
    if(_loops.empty()) {
        return vector<EventLoop*>(1, _baseLoop);
    } else{
        return _loops;
    }
}