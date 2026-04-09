//
// Created by harryqk on 3/27/26.
//

#ifndef SINAIECHO_CHANNEL_H
#define SINAIECHO_CHANNEL_H
#pragma once
#include <functional>
#include <cstdint>
namespace SinaiEcho
{
    class EventLoop;

    class Channel
    {
    public:
        using EventCallback = std::function<void()>;

        Channel(EventLoop* loop, int fd);
        ~Channel();

        void HandleEvent();

        void SetReadCallback(EventCallback cb) { ReadCallback = cb; }
        void SetWriteCallback(EventCallback cb) { WriteCallback = cb; }

        int GetFd() const { return fd; }

        void EnableReading();
        void EnableWriting();
        void DisableWriting();
        void DisableReading();

        int Events_; // 想要注册的事件
        bool IsReading() const { return Events_ & kReadEvent; }
        bool IsWriting() const { return Events_ & kWriteEvent; }

        void SetRevents(uint32_t revents);
        uint32_t GetRevents() const;
        static const uint32_t kReadEvent = 0x01;
        static const uint32_t kWriteEvent = 0x02;
    private:
        EventLoop* Loop;
        int fd;


        uint32_t REvents_;// Poller 返回实际发生的事件
        EventCallback ReadCallback;
        EventCallback WriteCallback;
    };
}

#endif //SINAIECHO_CHANNEL_H
