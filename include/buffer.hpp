#ifndef BUFFER_HPP
#define BUFFER_HPP
#include "core.hpp"
class Buffer{
    public:
        static const size_t kCheapPrepend = 8;//初始预留的prependabel空间大小
        static const size_t kInitialSize = 1024;

        explicit Buffer(size_t initalSize = kInitialSize)
            : buffer_(kCheapPrepend + initalSize)
            , readerIndex_(kCheapPrepend)
            , writerIndex_(kCheapPrepend){}
        // 计算缓冲区中未处理字符
        size_t readableBytes() const ;
        // 计算缓冲区中仍能写入字符数量
        size_t writableBytes() const ;
        // 返回缓冲区中已读数据空间大小
        size_t prependableBytes() const ;

        // 返回缓冲区中可读数据的起始地址
        const char *peek() const ;
        // 回收已处理数据部分缓冲区
        void retrieve(size_t len); 
        void retrieveAll();
        string retrieveAllAsString() ;
        string retrieveAsString(size_t len);

        // 判断缓冲区剩余部分是否足够
        void ensureWritableBytes(size_t len);
        // 添加数据到缓冲区指定地址
        void append(const char *data, size_t len);
        // 返回缓冲区可写入部分首地址
        char *beginWrite() ;
        const char *beginWrite() const ;

        // 处理与fd的交互
        ssize_t readFd(int fd, int *saveErrno);
        ssize_t writeFd(int fd, int *saveErrno);
    private:
        char *begin() ;
        const char *begin() const ;
        void makeSpace(size_t len) ;

    private:    
        std::vector<char> buffer_;
        size_t readerIndex_;
        size_t writerIndex_;
};  
#endif