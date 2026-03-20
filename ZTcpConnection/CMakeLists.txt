cmake_minimum_required(VERSION 3.15)
project(TcpConnection)

set(SRC_LIST TcpConnection.cpp)

add_library(tcpconnection ${SRC_LIST})

target_include_directories(tcpconnection PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(tcpconnection PRIVATE inet_address buffer timestamp)