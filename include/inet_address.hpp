#ifndef INET_ADDRESS_HPP
#define INET_ADDRESS_HPP
#include "core.hpp"
#include <arpa/inet.h>
#include <netinet/in.h>

class InetAddress {
    public:
        explicit InetAddress(uint16_t port = 0, string ip = "127.0.0.1");
        explicit InetAddress(const sockaddr_in &addr): addr_(addr) {}
        string toIp() const;
        string toInPort() const;
        uint16_t toPort() const;

        const sockaddr_in *getSockAddr() const;
        void setSockAddr(const sockaddr_in &addr); 
    private:
        sockaddr_in addr_;
};

#endif