cmake_minimum_required(VERSION 3.15)
project(InetAddress)

set(SRC_LIST inet_address.cpp)

add_library(inet_address ${SRC_LIST})

target_include_directories(inet_address PRIVATE ${CMAKE_SOURCE_DIR}/include)