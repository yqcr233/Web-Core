#include "current_thread.hpp"

__thread int t_cacheTid;

void cacheTid(){
    if(t_cacheTid == 0) {
        t_cacheTid = static_cast<pid_t>(syscall(SYS_gettid));
    }
}

int tid(){
    // __builtin_expect在底层让编译器预测t_cacheTid == 0的结果偏向于假(因为线程只有刚开始会调用一次if)
    // 会将偏向于假的代码放在连续内存减少跳转，而把为真时的代码放在较远的内存。
    if(__builtin_expect(t_cacheTid == 0, 0)) {
        cacheTid();
    }
    return t_cacheTid;
}