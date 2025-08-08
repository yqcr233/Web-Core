#ifndef EPOLLPOLLER_HPP
#define EPOLLPOLLER_HPP
#include "core.hpp"
#include "poller.hpp"
#include "timestamp.hpp"

class Channel;

class EpollPoller:public Poller{
    public:
        EpollPoller(EventLoop* loop);
        ~EpollPoller() override;
        TimeStamp poll(int timeoutMs, vector<Channel*> *activesChannels) override;
        void updateChannel(Channel *channel) override;
        void removeChannel(Channel *channel) override;
    private:
        static const int KInitEventListSize = 16;
        int epollfd_; // epoll_create创建返回的fd
        vector<epoll_event> events_; // 存放epoll_wait返回所有发生的事件的文件描述符事件集
    private:
        void fillActiveChannels(int numEvents, vector<Channel*> *activeChannels) const;
        void update(int operation, Channel* channel);
};
#endif