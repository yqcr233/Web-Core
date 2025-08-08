#ifndef LOGGER_HPP
#define LOGGER_HPP
#include "core.hpp"
#include "noncopyable.hpp"
#include "timestamp.hpp"
// 使用do - while(0) 形式是为了避免直接使用{}后在代码中按函数形式调用宏时多余的;
// 如：LOG_INFO("hello"); -->  do{ ... }while(0); 
// 如果直接使用{}形式      -->  { ... };  多余了;号
#define LOG_INFO(logmsgFormat, ...) \
    do{ \
        Logger& logger = Logger::Instance(); \
        logger.setLogLevel(INFO); \
        char buf[1024] = {0};    \
        snprintf(buf, 1024, logmsgFormat, ##__VA_ARGS__);    \
        logger.log(buf);    \
    }while(0)

#define LOG_ERROR(logmsgFormat, ...) \
    do{ \
        Logger& logger = Logger::Instance(); \
        logger.setLogLevel(ERROR); \
        char buf[1024] = {0};    \
        snprintf(buf, 1024, logmsgFormat, ##__VA_ARGS__);    \
        logger.log(buf);    \
    }while(0)

#define LOG_FATAL(logmsgFormat, ...) \
    do{ \
        Logger& logger = Logger::Instance(); \
        logger.setLogLevel(FATAL); \
        char buf[1024] = {0};   \
        snprintf(buf, 1024, logmsgFormat, ##__VA_ARGS__);    \
        logger.log(buf);    \
    }while(0)

#ifdef MUDEBUG // debug模式实现完成调试日志定义
#define LOG_DEBUG(logmsgFormat, ...) \
    do{ \
        Logger& logger = Logger::Instance(); \
        logger.setLogLevel(DEBUG); \
        char buf[1024] = {0};    \
        snprintf(buf, 1024, logmsgFormat, ##__VA_ARGS__);    \
        logger.log(buf);    \
    }while(0)
#else // 发布模式，设置调试日志为空
#define LOG_DEBUG(logmsgFormat, ...)
#endif

enum LogLevel{
    INFO,       // 普通信息
    ERROR,      // 错误信息
    FATAL,      // 致命错误
    DEBUG       // 调试信息
};

class Logger: noncopyable{
    public:
        static Logger &Instance(); // 获取单例
        void setLogLevel(int level); // 设置日志级别
        void log(string msg); // 写日志
    private:
        int logLevel_;
};
#endif