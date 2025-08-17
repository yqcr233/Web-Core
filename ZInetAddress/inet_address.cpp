#include "inet_address.hpp"

InetAddress::InetAddress(uint16_t port = 0, string ip = "127.0.0.1"){
    memset(&addr_, 0, sizeof(addr_));
    addr_.sin_family = AF_INET;
    addr_.sin_port = htons(port);
    addr_.sin_addr.s_addr = inet_addr(ip.c_str());    
}
string InetAddress::toIp() const{
    char buf[64] = {0};
    // 将ip地址从二进制形式转换为点分十进制形式
    inet_ntop(AF_INET, &addr_.sin_addr, buf, sizeof(buf));
    return buf;
}
string InetAddress::toInPort() const{
    char buf[64] = {0};
    inet_ntop(AF_INET, &addr_.sin_addr, buf, sizeof(buf));
    size_t end = strlen(buf);
    uint16_t port = ntohs(addr_.sin_port);
    sprintf(buf + end, ":%u", port);
    return buf;
}
uint16_t InetAddress::toPort() const{
    return ntohs(addr_.sin_port);
}
const sockaddr_in *InetAddress::getSockAddr() const{
    return &addr_;
}
void InetAddress::setSockAddr(const sockaddr_in &addr){
    addr_ = addr;
}