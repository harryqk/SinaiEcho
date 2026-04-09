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
            : Loop(loop), fd_(Sock->GetFileDescriptor()), SSock(std::move(Sock))
    {
        //SetNonBlock(fd_);
        channel = new Channel(loop, fd_);

        //channel->EnableReading();
        //channel->EnableWriting();
        channel->SetReadCallback([this]() {
            HandleRead();
        });
        channel->SetWriteCallback([this]() {
            HandleWrite();
        });

        Loop->AddChannel(channel);
    }

    NetConnection::~NetConnection()
    {
        //close(fd_);
        channel->DisableWriting();
        channel->DisableReading();
        Loop->RemoveChannel(channel);
        delete channel;

    }

// ================= 对外接口 =================
    void NetConnection::Send(std::string& Msg)
    {
        Loop->RunInLoop([this, Msg]() {
            SendInLoop(Msg.data());
        });
    }

    void NetConnection::Close()
    {
        Loop->RunInLoop([this]() {
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
            if (InputBuffer.size()  - ReadIndex < 4)
                break;
            int len = SocketUtil::BytesToInt((byte *)InputBuffer.data()+ ReadIndex);

            if (InputBuffer.size()  - ReadIndex < 4 + len)
                break;
            std::string msg = InputBuffer.substr(ReadIndex + 4, len);
            NetMessage message;
            message.id = 0;    // 可以自增或全局生成
            message.type = 0;       // 可根据协议设置类型
            message.Data = msg;
            message.fd = SSock->GetFileDescriptor();
            if (OnMessage) OnMessage(message);
            //std::cout << "recv: " << msg << " connection fd:" << SSock->GetFileDescriptor() << std::endl;
            ReadIndex += 4 + len;

            if (ReadIndex > 1024)
            {
                InputBuffer.erase(0, ReadIndex);
                ReadIndex = 0;
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


        channel->DisableWriting();
        channel->DisableReading();
        Loop->RemoveChannel(channel);
        SSock->Close();
        //close(fd_);
        // 通知上层删除
        if (CloseCallback_)
        {
            CloseCallback_(this);
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
        if (!channel->IsWriting())
        {
            ssize_t n = SSock->Send(OutputBuffer.data(), OutputBuffer.size(), 0);
            if (n > 0)
            {
                OutputBuffer.erase(0, n);
                if (!OutputBuffer.empty())
                    channel->EnableWriting();
            }
            else if (n < 0 && (errno == EAGAIN || errno == EWOULDBLOCK))
            {
                channel->EnableWriting();
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
                channel->DisableWriting();
                channel->EnableReading();
                std::cout << "HandleWrite connect success\n";
                if(ConnectedCallback)
                {
                    ConnectedCallback(this);
                }

            }
            else
            {
                std::cout << "HandleWrite connect fail\n";
                channel->DisableWriting();
            }
        }
        else if (state == kConnected)
        {
            if (OutputBuffer.empty())
            {
                channel->DisableWriting();
                return;
            }
            ssize_t n = SSock->Send(OutputBuffer.data(), OutputBuffer.size(), 0);

            if (n > 0)
            {
                OutputBuffer.erase(0, n);
                if (OutputBuffer.empty())
                {
                    channel->DisableWriting();
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
        return channel;
    }

    void NetConnection::SetState(State st)
    {
        state = st;
    }


}
