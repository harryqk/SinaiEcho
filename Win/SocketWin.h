//
// Created by harryqk on 7/17/22.
//

#ifndef SINAIECHO_SCPPSOCKETWIN_H
#define SINAIECHO_SCPPSOCKETWIN_H
#include "../Socket.h"
namespace SinaiEcho
{
    class SocketWin :public Socket{
    public:
        SocketWin(SocketAddressFamily AddressFamily, SocketType Type, SocketProtocol Protocol);
        SocketWin();
        virtual ~SocketWin();
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



#endif //SINAIECHO_SCPPSOCKETWIN_H
