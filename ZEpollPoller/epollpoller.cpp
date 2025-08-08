#include "epollpoller.hpp"
#include "logger.hpp"
#include "channel.hpp"
#include "errno.h"
#include "unistd.h"

const int KNew = -1;      // 某个channel还未添加到poller
const int KAdded = 1;    // 某个channel已添加到poller
const int KDeleted = 2;   // channel已被poller删除

EpollPoller::EpollPoller(EventLoop* loop): 
            Poller(loop), epollfd_(epoll_create1(EPOLL_CLOEXEC)), events_(KInitEventListSize)
{
    if(epollfd_ < 0) {
        LOG_FATAL("epoll_create error: %d\n", errno);
    }
}
EpollPoller::~EpollPoller() {
    close(epollfd_);
}
TimeStamp EpollPoller::poll(int timeoutMs, vector<Channel*> *activesChannels) {
    LOG_INFO("func = %s => fd total count: %lu\n", __FUNCTION__, channels_.size()); // __FUNCTION__是当前函数的名字
    int numEvent = epoll_wait(epollfd_, &*events_.begin(), static_cast<int>(events_.size()), timeoutMs);
    int saveErron = errno; // 及时记录错误避免后续操作中被修改
    TimeStamp now(TimeStamp::now());

    if(numEvent > 0) {
        LOG_INFO("%d events happend.\n", numEvent);
        fillActiveChannels(numEvent, activesChannels);
        if(numEvent == events_.size()) { // 一次响应事件太多则扩容
            events_.resize(events_.size() * 2);
        }
    }else if(numEvent == 0){
        LOG_INFO("%s timeout!\n", __FUNCTION__);
    }else{ 
        if(saveErron != EINTR) {
            errno = saveErron;
            LOG_ERROR("EPollPoller::poll() error!\n");
        }
    }
    return now;
}
void EpollPoller::updateChannel(Channel *channel) {
    const int index = channel->index();
    LOG_INFO("func = %s => fd = %d events = %d index = %d\n", __FUNCTION__, channel->fd(), channel->events(), index);

    if(index == KNew || index == KDeleted) { // channel没加入poller
        if(index == KNew) {
            int fd = channel->fd();
            channels_[fd] = channel;
        }
        channel->set_index(KAdded);
        update(EPOLL_CTL_ADD, channel);
    } else{ // 已注册则判断需移除或修改事件
        int fd = channel->fd();
        if(channel->isNoneEvent()) {
            update(EPOLL_CTL_DEL, channel);
            channel->set_index(KDeleted);
        } else{
            update(EPOLL_CTL_MOD, channel);
        }
    }
}
void EpollPoller::removeChannel(Channel *channel) {
    int fd = channel->fd();
    auto it = channels_.find(fd);
    if(it != channels_.end()) {
        channels_.erase(fd);
        LOG_INFO("func=%s => fd=%d\n", __FUNCTION__, fd);
        int index = channel->index();
        if(index == KAdded) {
            update(EPOLL_CTL_DEL, channel);
        }
        channel->set_index(KNew);
    }
}
// 填写活跃的链接
void EpollPoller::fillActiveChannels(int numEvents, vector<Channel*> *activeChannels) const{
    for(int i = 0; i < numEvents; i++) {
        Channel *channel = static_cast<Channel*>(events_[i].data.ptr);
        channel->set_revents(events_[i].events);
        activeChannels->push_back(channel);
    }
}
void EpollPoller::update(int operation, Channel* channel){
    epoll_event event;
    memset(&event, 0, sizeof(event));

    int fd = channel->fd();
    event.events = channel->events();
    event.data.fd = fd;
    event.data.ptr = channel;

    if(epoll_ctl(epollfd_, operation, fd, &event) < 0) {
        if (operation == EPOLL_CTL_DEL){
            LOG_ERROR("epoll_ctl del error:%d\n", errno);
        }
        else{
            LOG_FATAL("epoll_ctl add/mod error:%d\n", errno);
        }
    }
}