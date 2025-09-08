cmake_minimum_required(VERSION 3.15)
project(Test)

set(SRC_LIST test.cpp)

add_library(test ${SRC_LIST})

target_include_directories(test PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(test PRIVATE 
    channel 
    epollpoller 
    event_loop 
    inet_address
    acceptor
    socket
    buffer
)