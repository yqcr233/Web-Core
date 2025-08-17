#ifndef ACCEPTOR_HPP
#define ACCEPTOR_HPP
#include "core.hpp"
#include "noncopyable.hpp"
class EventLoop;
class InetAddress;

class Acceptor : noncopyable{
    public:
        Acceptor();
    private:
};

#endif