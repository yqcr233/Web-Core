#ifndef CALLBACK_HPP
#define CALLBACK_HPP
#include "core.hpp"

class Buffer;
class TcpConnection;
class TimeStamp;

using TcpConnectionPtr = shared_ptr<TcpConnection>;
using ConnectionCallback = function<void(const TcpConnectionPtr &)>;
using CloseCallback = function<void(const TcpConnectionPtr &)>;
using WriteCompleteCallback = function<void(const TcpConnectionPtr &)>;
using HighWaterMarkCallback = function<void(const TcpConnectionPtr &, size_t)>;
using MessageCallback = function<void(const TcpConnectionPtr&, Buffer*, TimeStamp)>;

#endif