cmake_minimum_required(VERSION 3.15)
project(TcpServer)

set(SRC_LIST tcp_server.cpp)

add_library(tcp_server ${SRC_LIST})

target_include_directories(tcp_server PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(tcp_server PRIVATE 
    timestamp 
    test 
    channel 
    epollpoller 
    event_loop 
    poller 
    logger 
    current_thread
    inet_address
    socket
    acceptor
    buffer
    thread_
    tcpconnection
)