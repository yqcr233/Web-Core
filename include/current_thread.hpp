#ifndef CURRENT_THREAD_HPP
#define CURRENT_THREAD_HPP
#include "core.hpp"
#include "unistd.h"
#include <sys/syscall.h>

extern __thread int t_cacheTid; // 缓存tid，避免重复进行系统调用

void cacheTid();

int tid();

#endif