//
// Created by harryqk on 3/27/26.
//

#ifndef SINAIECHO_WAKEUP_H
#define SINAIECHO_WAKEUP_H


#pragma once
namespace SinaiEcho
{
    class Wakeup
    {
    public:
        virtual ~Wakeup() = default;

        // 获取用于注册到 Poller 的 fd（或读端）
        virtual int GetFd() const = 0;

        // 跨线程唤醒 EventLoop
        virtual void Wake() = 0;

        // 在 EventLoop 线程中处理唤醒事件（清空）
        virtual void HandleRead() = 0;
    };
}


#endif //SINAIECHO_WAKEUP_H
