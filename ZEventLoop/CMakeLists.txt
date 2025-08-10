cmake_minimum_required(VERSION 3.15)
project(EventLoop)

set(SRC_LIST event_loop.cpp)

add_library(event_loop ${SRC_LIST})

target_include_directories(event_loop PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(event_loop PRIVATE current_thread)