#ifndef SOCKET_HPP
#define SOCKET_HPP
#include "core.hpp"
#include "noncopyable.hpp"

class InetAddress;
class Socket : noncopyable{
    public:
        explicit Socket(int sockfd) : sockfd_(sockfd){}
        ~Socket();
        int fd() const;
        void bindAddress(const InetAddress& local_addr);
        void listen();
        int accept(InetAddress *peer_addr);

        void shutdownWrite();        // 设置半关闭

        void setTCPNoDelay(bool on); // 设置Nagle算法
        void setReuseAddr(bool on);  // 设置地址复用
        void setReusePort(bool on);  // 设置端口复用
        void setKeepAlive(bool on);  // 设置长连接
    private:
        const int sockfd_;
};
#endif