#ifndef CHANNEL_HPP
#define CHANNEL_HPP
#include "core.hpp"
#include "noncopyable.hpp"
#include "timestamp.hpp"
// 前向声明
class EventLoop;
/**
 * sockfd的具体封装类，封装了sockfd和其感兴趣的event 如EPOLLIN、EPOLLOUT事件 还绑定了poller返回的具体事件
 */
class Channel{
    public:
        Channel(EventLoop* loop, int fd);
        ~Channel();
        void handleEvent(TimeStamp recvtime); // poller通过后具体处理事件的访问，通过EventLoop::loop调用
        // 设置回调函数
        void setReadCallback(function<void(TimeStamp)> cb);
        void setWriteCallback(function<void()> cb);
        void setCloseCallback(function<void()> cb);
        void setErrorCallback(function<void()> cb);
        // 延长持有channel的对象的生命周期，从而安全调用回调
        void tie(const shared_ptr<void> &);
        // getter 和 setter
        int fd() const;
        int events() const;
        void set_revents(int revt);
        int index();
        void set_index(int idx);
        // 设置fd事件状态
        void enableReading();
        void disableReading();
        void enableWriting();
        void disableWriting();
        void disableAll();
        // 获取事件状态
        bool isNoneEvent();
        bool isWriting();
        bool isReading();
        // one loop per thread
        EventLoop* ownerLoop();
        void remove();
    private:
        static const int KNoneEvent, KReadEvent, KWriteEvent;
        EventLoop* loop_;
        const int fd_;
        int events_;
        int revents_;
        int index_; // 该channel在poller中的状态
        // 获取上层对象
        weak_ptr<void> tie_;
        bool tied_;
        // 事件回调操作
        function<void(TimeStamp)> ReadCallBack;
        function<void()> WriteCallBack;
        function<void()> CloseCallBack;
        function<void()> ErrorCallBack;
    private:
        void update(); //添加channel到eventLoop的待更新列表
        void handleEventWithGuard(TimeStamp recvTime); // 事件就绪时安全调用事件回调
};
#endif