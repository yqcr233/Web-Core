cmake_minimum_required(VERSION 3.15)
project(Acceptor)

set(SRC_LIST acceptor.cpp)

add_library(acceptor ${SRC_LIST})

target_include_directories(acceptor PRIVATE ${CMAKE_SOURCE_DIR}/include)