#include "test.hpp"
#include "inet_address.hpp"
#include "socket_.hpp"
#include "acceptor.hpp"
#include "buffer.hpp"
void test_channel(){
    
}

void test_poller(){

}

void test_eventloop(){
    EventLoop loop{};
    const InetAddress inet;
    Socket sock{0};
    Acceptor ac{&loop, inet, true};
}

void test_buffer(){
    Buffer bf;
}
