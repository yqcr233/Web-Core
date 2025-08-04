#include "timestamp.hpp"

TimeStamp::TimeStamp(): microSecondsSinceEpoch_(0){}

TimeStamp::TimeStamp(int64_t microSecondsSinceEpoch): microSecondsSinceEpoch_(microSecondsSinceEpoch){}

TimeStamp TimeStamp::now(){// 获取当前时间戳并封装为TimeStamp
    return TimeStamp(time(NULL));
} 

string TimeStamp::ToString() const{// 将时间戳转换成字符串
    char buf[128] = {0};
    tm *tm_time = localtime(&microSecondsSinceEpoch_); // 转换为本地时间
    snprintf(buf, 128, "%4d/%02d/%02d %02d:%02d:%02d", 
        tm_time->tm_year + 1900, // 年份（从1900起）
        tm_time->tm_mon + 1,     // 月份（0-11）
        tm_time->tm_mday,        // 日（1-31）
        tm_time->tm_hour,        // 时（0-23）
        tm_time->tm_min,         // 分（0-59）
        tm_time->tm_sec);        // 秒（0-60，考虑闰秒）
    // fprintf(stdout, "%s\n", buf);
    return buf;
} 