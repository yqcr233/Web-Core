#include "channel.hpp"
#include "event_loop.hpp"
#include "logger.hpp"

const int Channel::KNoneEvent = 0; // 空事件
const int Channel::KReadEvent = EPOLLIN | EPOLLPRI; // 空事件
const int Channel::KWriteEvent = EPOLLOUT; // 空事件

Channel::Channel(EventLoop* loop, int fd): 
            loop_(loop), fd_(fd), events_(0), revents_(0), index_(-1), tied_(false) {}

Channel::~Channel() {}

void Channel::handleEvent(TimeStamp recvtime){// poller通过后具体处理事件的访问，通过EventLoop::loop调用
    if(tied_) { // 有上层TcpConnection对象
        shared_ptr<void> guard = tie_.lock();
        if(guard) {
            handleEventWithGuard(recvtime);
        }
        // 上层对象已被释放则不做处理
    }
    else{
        handleEventWithGuard(recvtime);
    }
} 
// 设置回调函数
void Channel::setReadCallback(function<void(TimeStamp)> cb){
    ReadCallBack = move(cb);
}
void Channel::setWriteCallback(function<void()> cb){
    WriteCallBack = move(cb);
}
void Channel::setCloseCallback(function<void()> cb){
    CloseCallBack = move(cb);
}
void Channel::setErrorCallback(function<void()> cb){
    ErrorCallBack = move(cb);
}
// 延长持有channel的对象的生命周期，从而安全调用回调
void Channel::tie(const shared_ptr<void> &obj){
    tie_ = obj;
    tied_ = true;
}
// getter 和 setter
int Channel::fd() const{
    return fd_;
}
int Channel::events() const{
    return events_;
}
void Channel::set_revents(int revt){
    revents_ = revt;
}
int Channel::index(){
    return index_;
}
void Channel::set_index(int idx){
    index_ = idx;
}
// 设置fd事件状态
void Channel::enableReading(){
    events_ |= KReadEvent; 
    update();
}
void Channel::disableReading(){
    events_ &= ~KReadEvent; 
    update();
}
void Channel::enableWriting(){
    events_ |= KWriteEvent;
    update();
}
void Channel::disableWriting(){
    events_ &= ~KWriteEvent;
    update();
}
void Channel::disableAll(){
    events_ = KNoneEvent;
    update();
}
// 获取事件状态
bool Channel::isNoneEvent(){
    return events_ == KNoneEvent;
}
bool Channel::isWriting(){
    return events_ & KWriteEvent;
}
bool Channel::isReading(){
    return events_ & KReadEvent;
}
// one loop per thread
EventLoop* Channel::ownerLoop(){
    return loop_;
}
void Channel::remove(){
    loop_->removeChannel(this);
}
void Channel::update(){//添加channel到eventLoop的待更新列表
    loop_->updateChannel(this);
}
void Channel::handleEventWithGuard(TimeStamp recvTime){// 事件就绪时安全调用事件回调
    // 错误
    if(revents_ & EPOLLERR) {
        if(ErrorCallBack) {
            ErrorCallBack();
        }
    }
    // 读
    if(revents_ & (EPOLLIN | EPOLLPRI)) {
        if(ReadCallBack) {
            ReadCallBack(recvTime);
        }
    }
    // 写
    if(revents_ & EPOLLOUT) {
        if(WriteCallBack) {
            WriteCallBack();
        }
    }
    // 关闭 
    if(revents_ & EPOLLHUP && !(revents_ & EPOLLIN)) { // 处理挂起事件，且无读事件
        if(CloseCallBack) {
            CloseCallBack();
        }
    }
}