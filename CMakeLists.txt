cmake_minimum_required(VERSION 3.15)
project(WebCore)

set(SRC_LIST main.cpp)

add_executable(web_core ${SRC_LIST})

add_subdirectory(ZTimeStamp)
add_subdirectory(ZTest)
add_subdirectory(ZChannel)
add_subdirectory(ZPoller)
add_subdirectory(ZEpollPoller)
add_subdirectory(ZEventLoop)
add_subdirectory(ZLog)
add_subdirectory(ZCurrentThread)
add_subdirectory(ZSocket)
add_subdirectory(ZAcceptor)
add_subdirectory(ZInetAddress)
add_subdirectory(ZBuffer)

target_include_directories(web_core PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(web_core PRIVATE 
    timestamp 
    test 
    channel 
    epollpoller 
    event_loop 
    poller 
    logger 
    current_thread
    inet_address
    socket
    acceptor
    buffer
)
