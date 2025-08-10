cmake_minimum_required(VERSION 3.15)
project(EpollPoller)

set(SRC_LIST epollpoller.cpp)

add_library(epollpoller ${SRC_LIST})

target_include_directories(epollpoller PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(epollpoller PRIVATE timestamp poller logger)