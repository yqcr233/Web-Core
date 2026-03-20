#include "tcp_server.hpp"
#include "event_loop.hpp"
#include "timestamp.hpp"
#include "tcp_connection.hpp"
#include "callbacks.hpp"
#include "buffer.hpp"
#include <iostream>
#include <functional>
#include <string>

using namespace std;
/**
 * muduo开发服务器程序
 * 1、组合TcpServer对象。
 * 2、创建EventLoop事件循环对象的指针
 * 3、根据TcpServer的构造函数，设置服务器的构造函数，便于TcpServer对象初始化
 * 4、注册处理连接的回调函数和处理读写事件的回调函数
 * 5、设置合适服务端线程数量
 */
class EchoServer
{
public:
    EchoServer(EventLoop *loop,
               const InetAddress &listenAddr,
               const string &nameArg) : _server(loop, listenAddr, nameArg), _loop(loop)
    {
        /**
         * 注册用户连接创建和断开回调
         */
        _server.setConnectionCallback(bind(&EchoServer::onConnection, this, placeholders::_1));
        /**
         * 注册用户读写事件回调
         */
        _server.setMessageCallback(bind(&EchoServer::onMessage, this, placeholders::_1, placeholders::_2, placeholders::_3));
        /**
         * 设置线程数大于1时，会默认分出一个线程作为新用户连接线程，其他为工作线程
         */
        _server.setThreadNum(2);
    }

    /**
     * 开启事件循环
     */
    void start()
    {
        _server.start();
    }

private:
    void onConnection(const TcpConnectionPtr &conn)
    {
        if (conn->connected())
        {
            fprintf(stdout, "%s->%s\n", conn->peerAddress().toInPort().c_str(), conn->localAddress().toInPort().c_str());
        }
        else
        {
            fprintf(stdout, "%s->%s\n", conn->peerAddress().toInPort().c_str(), conn->localAddress().toInPort().c_str());
            conn->shutdown();         // close(fd);
        }
    }

    void onMessage(const TcpConnectionPtr &conn,
                   Buffer *buf,
                   TimeStamp time)
    {
        string res = buf->retrieveAllAsString();
        fprintf(stdout, "recv data:%s time:%s\n", res.c_str(), time.ToString().c_str());
        conn->send(res);
    }

    TcpServer _server;
    EventLoop *_loop; // 相当于epoll
};

int main(int argc, char const *argv[])
{
    EventLoop loop;
    InetAddress addr(9998, "127.0.0.1");
    EchoServer server(&loop, addr, "EchoServer");

    server.start();
    loop.loop();           // 等于epoll_wait，以阻塞的方式等待新用户连接，和已连接用户的读写事件等
    return 0;
}
