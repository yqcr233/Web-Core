cmake_minimum_required(VERSION 3.15)
project(Channel)

set(SRC_LIST channel.cpp)

add_library(channel ${SRC_LIST})

target_include_directories(channel PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(channel PRIVATE timestamp logger)