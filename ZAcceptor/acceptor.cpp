#include "acceptor.hpp"
#include "logger.hpp"
#include "inet_address.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <errno.h>
#include <unistd.h>

static int createNonBlocking() {
    int sockfd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP);
    if(sockfd < 0) {
        LOG_FATAL("%s:%s:%d listen socket create err:%d\n", __FILE__, __FUNCTION__, __LINE__, errno);
    }
    return sockfd;
}

Acceptor::Acceptor(EventLoop *loop, const InetAddress &listenAddr, bool reuseport):
                                loop_(loop), acceptsocket_(createNonBlocking())
                                , acceptchannel_(loop, acceptsocket_.fd()), listening_(false) 
{
    acceptsocket_.setReuseAddr(true);
    acceptsocket_.setReusePort(true);
    acceptsocket_.bindAddress(listenAddr);
    acceptchannel_.setReadCallback(bind(&Acceptor::handleRead, this));
}
Acceptor::~Acceptor(){
    acceptchannel_.disableAll();
    acceptchannel_.remove();
}
void Acceptor::setNewConnectionCallback(const function<void(int sockfd, const InetAddress)> &cb){
    newConnectionCallback = cb;
}
bool Acceptor::listenning() const{
    return listening_;
}
void Acceptor::listen(){
    listening_ = true;
    acceptsocket_.listen();
    acceptchannel_.enableReading();
}
void Acceptor::handleRead(){
    InetAddress peerAddr;
    int connfd = acceptsocket_.accept(&peerAddr);
    if(connfd >= 0) {
        if(newConnectionCallback){
            newConnectionCallback(connfd, peerAddr);
        }
        else{
            close(connfd);
        }
    }
    else{
        LOG_ERROR("%s:%s:%d accept err:%d\n", __FILE__, __FUNCTION__, __LINE__, errno);
        if(errno == EMFILE) {
            LOG_ERROR("%s:%s:%d sockfd reached limit\n", __FILE__, __FUNCTION__, __LINE__);
        }
    }
}