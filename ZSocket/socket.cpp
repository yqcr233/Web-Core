#include "socket_.hpp"
#include "logger.hpp"
#include "inet_address.hpp"
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/tcp.h>

Socket::~Socket(){
    close(sockfd_);
}
int Socket::fd() const{
    return sockfd_;
}
void Socket::bindAddress(const InetAddress& local_addr){
    if(bind(sockfd_, (sockaddr*)(local_addr.getSockAddr()), sizeof(sockaddr_in))){
        LOG_FATAL("bind sockfd:%d fail\n", sockfd_);
    }
}
void Socket::listen(){
    if(::listen(sockfd_, 1024)) { // 这里使用::listen()表示调用全局作用域下listen函数而不是当前类或命名空间中listen函数
        LOG_FATAL("listen sockfd:%d fail\n", sockfd_);
    }
}
int Socket::accept(InetAddress *peer_addr){
    sockaddr_in addr;
    socklen_t len = sizeof(addr);
    memset(&addr, 0, len);
    int connfd = accept4(sockfd_, (sockaddr *)&addr, &len, SOCK_NONBLOCK | SOCK_CLOEXEC);
    if(connfd >= 0) {
        peer_addr->setSockAddr(addr);
    }
    return connfd;
}
void Socket::shutdownWrite(){
    if(shutdown(sockfd_, SHUT_WR) < 0) {
        LOG_ERROR("shutdownWrite error");
    }
}
/**
 * 设置Nagle算法
 *  Nagle 算法用于减少网络上传输的小数据包数量
 * 将 TCP_NODELAY 设置为 1 可以禁用该算法，允许小数据包立即发送
 */
void Socket::setTCPNoDelay(bool on){
    int optval = on ? 1 : 0;
    setsockopt(sockfd_, IPPROTO_TCP, TCP_NODELAY, &optval, sizeof(optval));
}
/**
 * SO_REUSEADDR 允许一个套接字强制绑定到一个已被其他套接字使用的端口
 * 这对于需要重启并绑定到相同端口的服务器应用程序非常有用
 */
void Socket::setReuseAddr(bool on){
    int optval = on ? 1 : 0;
    setsockopt(sockfd_, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
}
/**
 * SO_REUSEPORT 允许同一主机上的多个套接字绑定到相同的端口号
 * 这对于在多个线程或进程之间负载均衡传入连接非常有用。
 */
void Socket::setReusePort(bool on){
    int optval = on ? 1 : 0;
    setsockopt(sockfd_, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval));
}
/**
 * SO_KEEPALIVE 启用在已连接的套接字上定期传输消息。如果另一端没有响应，则认为连接已断开并关闭。
 * 这对于检测网络中失效的对等方非常有用。
 */
void Socket::setKeepAlive(bool on){
    int optval = on ? 1 : 0;
    setsockopt(sockfd_, SOL_SOCKET, SO_KEEPALIVE, &optval, sizeof(optval));
}