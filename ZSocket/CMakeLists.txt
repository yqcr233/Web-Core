cmake_minimum_required(VERSION 3.15)
project(Socket)

set(SRC_LIST socket_.cpp)

add_library(socket ${SRC_LIST})

target_include_directories(socket PRIVATE ${CMAKE_SOURCE_DIR}/include)