#ifndef TCP_CONNECTION_HPP
#define TCP_CONNECTION_HPP
#include "core.hpp"
#include "noncopyable.hpp"
#include "inet_address.hpp"
#include "callbacks.hpp"
#include "buffer.hpp"
#include "timestamp.hpp"

class Channel;
class EventLoop;
class Socket;

/**
 * TcpServer => Acceptor => 有一个新用户连接，通过accept函数拿到connfd
 * => TcpConnection设置回调 => 设置到Channel => Poller => Channel回调
 **/
class TcpConnection : noncopyable, public enable_shared_from_this<TcpConnection>{
    public:
        TcpConnection(EventLoop* loop, const string &nameArg, int sockfd, const InetAddress &localAddr, const InetAddress &peerAddr);
        ~TcpConnection();

        EventLoop* getLoop() const {return _loop;}
        const string& name() const{return _name;}
        const InetAddress &localAddress() const{return _localAddr;}
        const InetAddress &peerAddress() const{return _peerAddr;}

        bool connected() const{ return _state == kConnected;}

        // 发送数据
        void send(const string& buf);
        // void sendFile(int fileDescriptor, off_t offset, size_t count);
        // 关闭半链接
        void shutdown();

        void setConnectionCallback(const ConnectionCallback& cb){_connectionCallback = cb;}
        void setMessageCallback(const MessageCallback& cb){_messageCallback = cb;}
        void setWriteCompleteCallback(const WriteCompleteCallback& cb){_writeCompleteCallback = cb;}
        void setCloseCallback(const CloseCallback& cb){_closeCallback = cb;}
        void setHighWaterMaskCallback(const HighWaterMarkCallback& cb){_highWaterMarkCallback = cb;}

        // 链接建立和销毁
        void connectEstablished();
        void connectDestroyed();

    private:
        enum _State{
            kDisconnected, // 已经断开链接
            kConnecting, // 正在链接
            kConnected, //已链接
            kDisconnecting //正在断开链接
        };

        void setState(_State state){_state = state;}
        void handleRead(TimeStamp Timestamp);
        void handleWrite();
        void handleClose();
        void handleError();

        void shutdownInLoop();
        void sendInLoop(const void* data, size_t len);
        // void sendFileInLoop(int fileDescriptor, off_t offset, size_t len);

        EventLoop* _loop;
        const string _name;
        atomic_int _state;
        bool _reading;
        unique_ptr<Socket>  _socket;
        unique_ptr<Channel> _channel;
        
        const InetAddress _localAddr;
        const InetAddress _peerAddr;
        // 设置回调函数
        ConnectionCallback _connectionCallback;
        MessageCallback _messageCallback;
        WriteCompleteCallback _writeCompleteCallback;
        HighWaterMarkCallback _highWaterMarkCallback;
        CloseCallback _closeCallback;
        size_t _highWaterMask;
        // 数据缓冲区
        Buffer _inputBuffer;
        Buffer _outputBuffer;
};
#endif