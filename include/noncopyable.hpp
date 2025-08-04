#ifndef NONCOPYABLE_HPP
#define NONCOPYABLE_HPP
/**
 * noncopyable被继承后 派生类对象可正常构造和析构 但派生类对象无法进行拷贝构造和赋值构造
 **/
class noncopyable
{
public:
    noncopyable(const noncopyable &) = delete;
    noncopyable &operator=(const noncopyable &) = delete;
protected: // 防止外部直接构建noncopyable实例
    noncopyable() = default;
    ~noncopyable() = default;
};
#endif
