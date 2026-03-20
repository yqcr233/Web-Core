#ifndef TCP_SERVER_HPP
#define TCP_SERVER_HPP
#include "core.hpp"
#include "noncopyable.hpp"
#include "event_loop.hpp"
#include "acceptor.hpp"
#include "inet_address.hpp"
#include "event_loop_thread_pool.hpp"
#include "callbacks.hpp"

class TcpServer
{
public:
    using ThreadInitCallback = function<void(EventLoop *)>;
    enum Option
    {
        kNoReusePort,
        kReusePort,
    };

    TcpServer(EventLoop *loop, const InetAddress &addr, const string nameArg, Option option = kNoReusePort);
    ~TcpServer();

    void setThreadInitCallback(const ThreadInitCallback& cb) { threadInitCallback = cb;}
    void setConnectionCallback(const ConnectionCallback& cb) { connectionCallback = cb;}
    void setMessageCallback(const MessageCallback& cb) { messageCallback = cb;}
    void setWriteCompleteCallback(const WriteCompleteCallback& cb) { writeCompleteCallback = cb;}

    void setThreadNum(int numThreads);

    void start();

private:
    void newConnection(int sockfd, const InetAddress &peerAddr);
    void removeConnection(const TcpConnectionPtr &conn);
    void removeConnectionInPool(const TcpConnectionPtr &conn);

    using ConnectionMap = unordered_map<string, TcpConnectionPtr>;

    EventLoop* loop;    // baseloop, 一般作为acceptor用来监听连接的循环
    
    const string ipPort;
    const string name;

    unique_ptr<Acceptor> acceptor;
    shared_ptr<EventLoopTreadPool> threadPool;
    ConnectionCallback connectionCallback;
    MessageCallback messageCallback;
    WriteCompleteCallback writeCompleteCallback;
    ThreadInitCallback threadInitCallback;
    
    atomic_int started;

    int nextConnId;
    ConnectionMap connections;
};
#endif