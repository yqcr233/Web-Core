#include "buffer.hpp"
#include <errno.h>
#include <sys/uio.h>
#include <unistd.h>

size_t Buffer::readableBytes() const {
    return writerIndex_ - readerIndex_;
}
size_t Buffer::writableBytes() const {
    return buffer_.size() - writerIndex_;
}
size_t Buffer::prependableBytes() const {
    return readerIndex_ - kCheapPrepend;
}
const char * Buffer::peek() const {
    return begin() + readerIndex_;
}
void Buffer::retrieve(size_t len){
    if (len < readableBytes()){
        readerIndex_ += len; 
    }
    else{
        retrieveAll();
    }
}
void Buffer::retrieveAll(){
    readerIndex_ = kCheapPrepend;
    writerIndex_ = kCheapPrepend;
}
string Buffer::retrieveAllAsString() {
    string result(peek(), readableBytes());
    retrieveAll();
    return result;
}
string Buffer::retrieveAsString(size_t len){
    string result(peek(), len);
    retrieve(len); 
    return result;
}
void Buffer::ensureWritableBytes(size_t len){
    if (writableBytes() < len){
        makeSpace(len); // 扩容
    }
}
void Buffer::append(const char *data, size_t len){
    ensureWritableBytes(len);
    copy(data, data+len, beginWrite());
    writerIndex_ += len;   
}
char *Buffer::beginWrite() {
    return begin() + writerIndex_;
}
const char *Buffer::beginWrite() const {
    return begin() + writerIndex_;
}
/**
 * inputBuffer_.readFd表示将对端数据读到inputBuffer_中，移动writerIndex_指针
 * outputBuffer_.writeFd标示将数据写入到outputBuffer_中，从readerIndex_开始，可以写readableBytes()个字节
 */
ssize_t Buffer::readFd(int fd, int *saveErrno){
    // 栈额外空间，用于从套接字往出读时，当buffer_暂时不够用时暂存数据，待buffer_重新分配足够空间后，在把数据交换给buffer_。
    char extrabuf[65536] = {0};
    /*
    struct iovec {
        ptr_t iov_base; // iov_base指向的缓冲区存放的是readv所接收的数据或是writev将要发送的数据
        size_t iov_len; // iov_len在各种情况下分别确定了接收的最大长度以及实际写入的长度
    };
    */
    // 使用iovec分配两个连续的缓冲区
    struct iovec vec[2];
    const size_t writable = writableBytes(); 
    // 第一块缓冲区，指向可写空间
    vec[0].iov_base = begin() + writerIndex_;
    vec[0].iov_len = writable;
    // 第二块缓冲区，指向栈空间
    vec[1].iov_base = extrabuf;
    vec[1].iov_len = sizeof(extrabuf);

    // 这里之所以说最多128k-1字节，是因为若writable为64k-1，那么需要两个缓冲区 第一个64k-1 第二个64k 所以做多128k-1
    // 如果第一个缓冲区>=64k 那就只采用一个缓冲区 而不使用栈空间extrabuf[65536]的内容
    const int iovcnt = (writable < sizeof(extrabuf)) ? 2 : 1;
    const ssize_t n = ::readv(fd, vec, iovcnt);
    if (n < 0)
    {
        *saveErrno = errno;
    }
    else if (n <= writable) // Buffer的可写缓冲区已经够存储读出来的数据了
    {
        writerIndex_ += n;
    }
    else // extrabuf里面也写入了n-writable长度的数据
    {
        writerIndex_ = buffer_.size();
        append(extrabuf, n - writable); // 对buffer_扩容 并将extrabuf存储的另一部分数据追加至buffer_
    }
    return n;
}
ssize_t Buffer::writeFd(int fd, int *saveErrno){
    ssize_t n = write(fd, peek(), readableBytes());
    if(n < 0) {
        *saveErrno = errno;
    }
    return n;
}
char *Buffer::begin() {
    return &*buffer_.begin();
}
const char *Buffer::begin() const {
    return &*buffer_.begin();
}
void Buffer::makeSpace(size_t len) {
    if (writableBytes() + prependableBytes() < len) // 也就是说 len > 已读 + writer的部分
    {
        buffer_.resize(writerIndex_ + len);
    }
    else { // 如果除去未读部分数据和预留空间外缓冲区足够，就将未读部分数据移动到数据部分头部，使得已读和未使用部分缓冲区合并
        size_t readable = readableBytes(); 
        std::copy(begin() + readerIndex_, begin() + writerIndex_, begin() + kCheapPrepend);
        readerIndex_ = kCheapPrepend;
        writerIndex_ = readerIndex_ + readable;
    }
}

