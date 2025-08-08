#ifndef POLLER_HPP
#define POLLER_HPP
#include "core.hpp"
#include "noncopyable.hpp"
#include "timestamp.hpp"

class Channel;
class EventLoop;
class Poller{
    public:
        Poller(EventLoop* loop);
        virtual ~Poller() = default;

        virtual TimeStamp poll(int timeoutMs, vector<Channel*> *activesChannels) = 0;
        virtual void updateChannel(Channel *channel) = 0;
        virtual void removeChannel(Channel *channel) = 0;
        
        bool hasChannel(Channel *channel) const;
        // 静态工厂方法，选择对应的poller派生类返回
        // 依赖倒置：依赖抽象而非实现
        static Poller* newDefaultPoller(EventLoop* loop);
    protected:
        unordered_map<int, Channel*> channels_;
    private:
        EventLoop* owner_loop;
};
#endif