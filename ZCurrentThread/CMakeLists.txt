cmake_minimum_required(VERSION 3.15)
project(CurrentThread)

set(SRC_LIST current_thread.cpp)

add_library(current_thread ${SRC_LIST})

target_include_directories(current_thread PRIVATE ${CMAKE_SOURCE_DIR}/include)