#include "logger.hpp"

Logger& Logger::Instance(){
    static Logger logger;
    return logger;
}
void Logger::setLogLevel(int level){
    logLevel_ = level;
}
void Logger::log(string msg){
    string pre = "";
    switch(logLevel_) {
        case INFO:
            pre = "[INFO]";
            break;
        case ERROR:
            pre = "[ERROR]";
            break;
        case FATAL:
            pre = "[FATAL]";
            break;
        case DEBUG:
            pre = "[DEBUG]";
            break;
        default:
            break;
    }
    // 打印日志信息
    cout << pre + TimeStamp::now().ToString() << " : " << msg << endl;
}