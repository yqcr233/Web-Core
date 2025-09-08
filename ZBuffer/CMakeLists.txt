cmake_minimum_required(VERSION 3.15)
project(Buffer)

set(SRC_LIST buffer.cpp)

add_library(buffer ${SRC_LIST})

target_include_directories(buffer PRIVATE ${CMAKE_SOURCE_DIR}/include)