#ifndef ACCEPTOR_HPP
#define ACCEPTOR_HPP
#include "core.hpp"
#include "noncopyable.hpp"
#include "socket_.hpp"
#include "channel.hpp"
class EventLoop;
class InetAddress;

class Acceptor : noncopyable{
    public:
        Acceptor(EventLoop *loop, const InetAddress &listenAddr, bool reuseport);
        ~Acceptor();
        void setNewConnectionCallback(const function<void(int sockfd, const InetAddress)> &cb);
        bool listenning() const;
        void listen();

    private:
        EventLoop *loop_;               // acceptor使用的loop就是用的mainloop
        Socket acceptsocket_;           // 用于接收新连接的socket
        Channel acceptchannel_;         // 用于监听新连接的channel
        function<void(int sockfd, const InetAddress&)> newConnectionCallback;     //  新连接的回调函数
        bool listening_;                   // 是否在监听
    private:
        void handleRead();              // 处理新用户的链接事件
};

#endif