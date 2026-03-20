cmake_minimum_required(VERSION 3.15)
project(Thread_)

set(SRC_LIST thread_.cpp)

add_library(thread_ ${SRC_LIST})

target_include_directories(thread_ PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(thread_ PRIVATE pthread)
