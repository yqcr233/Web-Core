#include "poller.hpp"
#include "channel.hpp"
#include "epollpoller.hpp"
Poller::Poller(EventLoop* loop) : owner_loop(loop) {}

bool Poller::hasChannel(Channel *channel) const{
    auto it = channels_.find(channel->fd());
    return it != channels_.end() && it->second == channel;
}

Poller* Poller::newDefaultPoller(EventLoop* loop){
    if(getenv("MUDUO_USE_POLL")) { // 如果没设置环境变量MUDUO_USE_POLL则返回默认poller实例(此处未设置)
        return nullptr;
    }
    else{
        return new EpollPoller(loop);
    }
}