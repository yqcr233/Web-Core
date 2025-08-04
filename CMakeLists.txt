cmake_minimum_required(VERSION 3.15)
project(WebCore)

set(SRC_LIST main.cpp)

add_executable(web_core ${SRC_LIST})

add_subdirectory(ZTimeStamp)

target_include_directories(web_core PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(web_core PRIVATE timestamp)
