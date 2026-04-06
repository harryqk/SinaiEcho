//
// Created by harryqk on 3/27/26.
//

#ifndef SINAIECHO_NETCONNECTION_H
#define SINAIECHO_NETCONNECTION_H


#pragma once

#include <string>
#include <functional>
#include "Socket.h"


namespace SinaiEcho
{
    class EventLoop;
    class Channel;
    enum State
    {
        kConnecting,
        kConnected,
        kDisconnected
    };

    class NetConnection
    {
    public:
        using EventCallBack = std::function<void(NetConnection*)>;

    public:
        // Socket 传入后 ownership 转移
        NetConnection(EventLoop* loop, std::unique_ptr<Socket> Sock);
        ~NetConnection();

        int GetFd() const { return fd_; }

        // 对外发送接口（线程安全）
        void Send(std::string& Msg);

        // 主动关闭
        void Close();

        // 设置关闭回调（给上层用，比如移除连接）
        void SetCloseCallback(EventCallBack cb)
        {
            CloseCallback_ = cb;
        }

        // 设置连接成功回调（给上层用，比如客户端提示）
        void SetConnectedCallback(EventCallBack cb)
        {
            ConnectedCallback = cb;
        }
        void SetState(State st);
        Channel* GetChannel();
    private:
        // IO线程执行
        void SendInLoop(const std::string& Msg);
        void HandleRead();
        void HandleClose();
        void HandleWrite();


    private:
        EventLoop* Loop;
        int fd_;

        Channel* channel;

        std::string WriteBuffer_;

        EventCallBack CloseCallback_;
        EventCallBack ConnectedCallback;
        std::unique_ptr<Socket> SSock;
        State state = kDisconnected;
        std::string InputBuffer;
        std::string OutputBuffer;
        int ReadIndex = 0;
    };
}
#endif //SINAIECHO_NETCONNECTION_H
