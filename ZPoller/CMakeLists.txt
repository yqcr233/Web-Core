cmake_minimum_required(VERSION 3.15)
project(Poller)

set(SRC_LIST poller.cpp)

add_library(poller ${SRC_LIST})

target_include_directories(poller PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(poller PRIVATE timestamp channel epollpoller)