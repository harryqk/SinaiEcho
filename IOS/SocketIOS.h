//
// Created by harryqk on 7/17/22.
//

#ifndef SINAIECHO_SCPPSOCKETIOS_H
#define SINAIECHO_SCPPSOCKETIOS_H
#include "../Socket.h"
namespace SinaiEcho
{
    class SocketIOS :public Socket
    {
    public:
        SocketIOS(SocketAddressFamily AddressFamily, SocketType Type, SocketProtocol Protocol);

        SocketIOS();
        virtual ~SocketIOS();
        virtual int Bind(int Port) override;
        virtual int Listen(int MaxConnect) override;
        virtual int Connect(sockaddr* Address) override;
        virtual SSocket Accept(sockaddr* Address) override;
        virtual SockSSize_t Send(const char* Buf, SockSize_t Len, int Flag) override;
        virtual SockSSize_t Read(char* Buf, SockSize_t Len, int Flag) override;
        virtual int SetNonBlockMode(bool NonBlock) override;
        virtual bool Close() override;
        virtual bool ShutDown() override;
        virtual Socket* Clone(SSocket NewSocket, sockaddr_in NewPeerAddress) override;
    };


}


#endif //SINAIECHO_SCPPSOCKETIOS_H
