#include "tcp_server.hpp"
#include "logger.hpp"
#include "tcp_connection.hpp"

static EventLoop *CheckNoNull(EventLoop *loop)
{
    if (loop == nullptr)
    {
        LOG_FATAL("%s:%s:%d mainLoop is null!\n", __FILE__, __FUNCTION__, __LINE__);
    }
    return loop;
}

TcpServer::TcpServer(EventLoop *loop, const InetAddress &addr, const string nameArg, Option option)
    : loop(CheckNoNull(loop)), ipPort(addr.toInPort()), name(nameArg), acceptor(new Acceptor(loop, addr, kReusePort)),
      threadPool(new EventLoopTreadPool(loop, name)), nextConnId(1), started(0)
{
    acceptor->setNewConnectionCallback(bind(&TcpServer::newConnection, this, placeholders::_1, placeholders::_2));
}

TcpServer::~TcpServer()
{
    for (auto &it : connections)
    {
        TcpConnectionPtr conn(it.second);
        it.second.reset(); // 让栈空间指针接管连接
        conn->getLoop()->runInLoop(bind(&TcpConnection::connectDestroyed, conn));
    }
}

void TcpServer::setThreadNum(int numThreads)
{
    threadPool->setThreadNum(numThreads);
}

void TcpServer::start()
{
    if (started++ == 0)    // 防止服务器重复启动
    { 
        threadPool->start(threadInitCallback);
        loop->runInLoop(bind(&Acceptor::listen, acceptor.get()));
    }
}

void TcpServer::newConnection(int sockfd, const InetAddress &peerAddr)
{
    // 轮询算法 选择一个subLoop 来管理connfd对应的channel
    EventLoop *ioLoop = threadPool->getNextLoop();
    char buf[64] = {0};
    snprintf(buf, sizeof buf, "-%s#%d", ipPort.c_str(), nextConnId);
    ++nextConnId;  // 这里没有设置为原子类是因为其只在mainloop中执行 不涉及线程安全问题
    std::string connName = name + buf;
    LOG_INFO("TcpServer::newConnection [%s] - new connection [%s] from %s\n",name.c_str(), connName.c_str(), peerAddr.toInPort().c_str());

    /**
     * 通过sockfd获取本机ip和端口信息
     */
    sockaddr_in local;
    memset(&local, 0, sizeof(local));
    socklen_t addrLen = sizeof(local);
    if(getsockname(sockfd, (sockaddr*)&local, &addrLen) < 0) {
        LOG_ERROR("sockets::getLocalAddr");
    }

    /**
     * 创建链接并设置相关回调
     */
    InetAddress localAddr(local);
    TcpConnectionPtr conn(new TcpConnection(ioLoop, connName, sockfd, localAddr, peerAddr));
    connections[connName] = conn;

    conn->setConnectionCallback(connectionCallback);
    conn->setMessageCallback(messageCallback);
    conn->setWriteCompleteCallback(writeCompleteCallback);
    conn->setCloseCallback(bind(&TcpServer::removeConnection, this, placeholders::_1));
    ioLoop->runInLoop(bind(&TcpConnection::connectEstablished, conn));
}

void TcpServer::removeConnection(const TcpConnectionPtr &conn)
{
    loop->runInLoop(bind(&TcpServer::removeConnectionInPool, this, conn));
}

void TcpServer::removeConnectionInPool(const TcpConnectionPtr &conn)
{
    LOG_INFO("TcpServer::removeConnectionInLoop [%s] - connection %s\n", name.c_str(), conn->name().c_str());

    connections.erase(conn->name());
    conn->getLoop()->queueInLLoop(bind(&TcpConnection::connectDestroyed, conn));
}
