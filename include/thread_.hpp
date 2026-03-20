#ifndef _THREAD_HPP
#define _THREAD_HPP
#include "core.hpp"
#include "noncopyable.hpp"

class Thread_: noncopyable{
    public:
        using ThreadFunc = function<void()>;
        explicit Thread_(ThreadFunc, const string &name = string());
        ~Thread_();

        void start();
        void join();

        // getter & setter
        bool started();
        pid_t tid() const;
        const string& name() const;
        static int numCreated();
    private:
        void setDefaultName();
    private:
        bool started_;
        bool joined_;
        shared_ptr<thread> thread_;
        pid_t tid_;                             // 线程创建时绑定
        ThreadFunc func_;                       // 线程回调
        string name_;
        static atomic_int numCreated_;
}; 
#endif

