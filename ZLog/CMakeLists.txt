cmake_minimum_required(VERSION 3.15)
project(Logger)

set(SRC_LIST logger.cpp)

add_library(logger ${SRC_LIST})

target_include_directories(logger PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(logger PRIVATE timestamp)