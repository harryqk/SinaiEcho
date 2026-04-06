//
// Created by harryqk on 3/27/26.
//

#include "NetConnection.h"
#include "EventLoop.h"
#include "Channel.h"
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <iostream>
#include "SocketUtil.h"

namespace SinaiEcho
{



// ================= 工具 =================
    static int SetNonBlock(int fd)
    {
        int flags = fcntl(fd, F_GETFL, 0);
        return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
    }

// ================= 构造 / 析构 =================
    NetConnection::NetConnection(EventLoop* loop, std::unique_ptr<Socket> Sock)
            : loop_(loop), fd_(Sock->GetFileDescriptor()), SSock(std::move(Sock))
    {
        //SetNonBlock(fd_);
        channel_ = new Channel(loop, fd_);

        //channel_->EnableReading();
        //channel_->EnableWriting();
        channel_->SetReadCallback([this]() {
            HandleRead();
        });
        channel_->SetWriteCallback([this]() {
            HandleWrite();
        });

        loop_->AddChannel(channel_);
    }

    NetConnection::~NetConnection()
    {
        //close(fd_);
        channel_->DisableWriting();
        channel_->DisableReading();
        loop_->RemoveChannel(channel_);
        delete channel_;

    }

// ================= 对外接口 =================
    void NetConnection::Send(std::string& Msg)
    {
        loop_->RunInLoop([this, Msg]() {
            SendInLoop(Msg.data());
        });
    }

    void NetConnection::Close()
    {
        loop_->RunInLoop([this]() {
            HandleClose();
        });
    }


// ================= 内部逻辑 =================

    void NetConnection::HandleRead()
    {

        char ReadBuf[1024];
        while (true)
        {
            ssize_t n = SSock->Read(ReadBuf, sizeof(ReadBuf), 0);
            if (n > 0)
            {
                InputBuffer.append(ReadBuf, n);
            }
            else if (n == 0)
            {
                HandleClose();
                return;
            }
            else
            {
                if (errno == EAGAIN)
                    break;
                if (errno == EINTR)
                    continue;

                HandleClose();
                return;
            }
        }


        while (true)
        {
            if (InputBuffer.size()  - readIndex < 4)
                break;
            int len = SocketUtil::BytesToInt((byte *)InputBuffer.data()+ readIndex);

            if (InputBuffer.size()  - readIndex < 4 + len)
                break;
            std::string msg = InputBuffer.substr(readIndex + 4, len);
            std::cout << "recv: " << msg << " connection fd:" << SSock->GetFileDescriptor() << std::endl;
            readIndex += 4 + len;

            if (readIndex > 1024)
            {
                InputBuffer.erase(0, readIndex);
                readIndex = 0;
            }
        }

    }

    void NetConnection::HandleClose()
    {
        //状态保护，防止重复调用
        if (state == kDisconnected)
            return;

        state = kDisconnected;
        std::cout << "connection closed fd=" << fd_ << std::endl;


        channel_->DisableWriting();
        channel_->DisableReading();
        loop_->RemoveChannel(channel_);
        SSock->Close();
        //close(fd_);
        // 通知上层删除
        if (closeCallback_)
        {
            closeCallback_(this);
        }

    }

    void NetConnection::SendInLoop(const std::string &Msg)
    {
        if (Msg.empty()) return;
        byte bytes[4];
        int Len = Msg.size();
        SocketUtil::IntToByte(Len, bytes);

// 直接写入 OutputBuffer
        OutputBuffer.append((char*)bytes, 4);
        OutputBuffer.append(Msg);

// 尝试发送 OutputBuffer 的内容
        if (!channel_->IsWriting())
        {
            ssize_t n = SSock->Send(OutputBuffer.data(), OutputBuffer.size(), 0);
            if (n > 0)
            {
                OutputBuffer.erase(0, n);
                if (!OutputBuffer.empty())
                    channel_->EnableWriting();
            }
            else if (n < 0 && (errno == EAGAIN || errno == EWOULDBLOCK))
            {
                channel_->EnableWriting();
            }
            else
            {
                HandleClose();
            }
        }
    }

    void NetConnection::HandleWrite()
    {
        std::cout << "HandleWrite\n";
        if (state == kDisconnected)
            return;

        if (state == kConnecting)
        {
            // 处理 connect 完成
            int err = 0;

            SSock->GetSocketError(err);

            if (err == 0)
            {
                state = kConnected;
                channel_->DisableWriting();
                channel_->EnableReading();
                std::cout << "HandleWrite connect success\n";
                if(connectedCallback)
                {
                    connectedCallback(this);
                }

            }
            else
            {
                std::cout << "HandleWrite connect fail\n";
                channel_->DisableWriting();
            }
        }
        else if (state == kConnected)
        {
            if (OutputBuffer.empty())
            {
                channel_->DisableWriting();
                return;
            }
            ssize_t n = SSock->Send(OutputBuffer.data(), OutputBuffer.size(), 0);

            if (n > 0)
            {
                OutputBuffer.erase(0, n);
                if (OutputBuffer.empty())
                {
                    channel_->DisableWriting();
                }
            }
            else if(n < 0)
            {
                if (errno == EAGAIN || errno == EWOULDBLOCK)
                {
                    // 下次写事件再写
                    return;
                }
                else
                {
                    HandleClose();
                }
            }
            else
            {
                // n == 0，当作异常处理
                HandleClose();
            }

        }
    }

    Channel *NetConnection::GetChannel()
    {
        return channel_;
    }

    void NetConnection::SetState(State st)
    {
        state = st;
    }


}
