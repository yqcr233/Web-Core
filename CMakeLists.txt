cmake_minimum_required(VERSION 3.15)
project(WebCore)

set(SRC_LIST main.cpp)

add_executable(web_core ${SRC_LIST})

target_include_directories(web_core PRIVATE ${CMAKE_SOURCE_DIR}/include)

