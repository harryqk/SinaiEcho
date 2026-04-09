//
// Created by harryqk on 7/17/22.
//

#ifndef SINAIECHO_SCPPSOCKETLINUX_H
#define SINAIECHO_SCPPSOCKETLINUX_H
#include "../Socket.h"
namespace SinaiEcho
{
    class SocketLinux :public Socket
    {
    public:
        SocketLinux(SocketAddressFamily AddressFamily, SocketType Type, SocketProtocol Protocol);

        SocketLinux();
        virtual ~SocketLinux();
        virtual int Bind(int Port) override;
        virtual int Listen(int MaxConnect) override;
        virtual int Connect(sockaddr* Address) override;
        virtual SSocket Accept(sockaddr* Address) override;
        virtual int Accept(int Fd) override;
        virtual SockSSize_t Send(const char* Buf, SockSize_t Len, int Flag) override;
        virtual SockSSize_t Read(char* Buf, SockSize_t Len, int Flag) override;
        virtual int SetNonBlockMode(bool NonBlock) override;
        virtual bool Close() override;
        virtual bool ShutDown() override;
        virtual Socket* Clone(SSocket NewSocket, sockaddr_in NewPeerAddress) override;
        virtual bool GetSocketError(int& err) override;
    };


}


#endif //SINAIECHO_SCPPSOCKETLINUX_H
