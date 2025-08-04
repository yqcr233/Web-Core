cmake_minimum_required(VERSION 3.15)
project(TimeStamp)

set(SRC_LIST timestamp.cpp)

add_library(timestamp ${SRC_LIST})

target_include_directories(timestamp PRIVATE ${CMAKE_SOURCE_DIR}/include)