#ifndef TIMESTAMP_HPP
#define TIMESTAMP_HPP
#include "core.hpp"
class TimeStamp{
    public:
        TimeStamp();
        explicit TimeStamp(int64_t microSecondsSinceEpoch);
        static TimeStamp now(); // 获取当前时间戳并封装为TimeStamp
        string ToString() const; // 将时间戳转换成字符串
    private:
        int64_t microSecondsSinceEpoch_; // tm存储自1900-01-01 00:00:00以来的微秒数
};
#endif