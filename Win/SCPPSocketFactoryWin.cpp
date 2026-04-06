//
// Created by harryqk on 7/17/22.
//

#include "SCPPSocketFactoryWin.h"
#include "SocketWin.h"



namespace SinaiEcho
{
    SCPPSocketFactoryWin::SCPPSocketFactoryWin()
    {
        std::printf("construct SCPPSocketFactoryWin\n");
    }

    SCPPSocketFactoryWin::~SCPPSocketFactoryWin()
    {
        std::printf("destruct SCPPSocketFactoryWin\n");
    }

    Socket* SCPPSocketFactoryWin::CreateSocket(SocketAddressFamily AddressFamily, SocketType Type, SocketProtocol Protocol)
    {
        SocketWin* Win = new SocketWin(AddressFamily, Type, Protocol);
        return Win;

    }

    sockaddr_in SCPPSocketFactoryWin::CreateAddress(const char *Address, int Port)
    {
        sockaddr_in server_addr;
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = inet_addr(Address);
        server_addr.sin_port = htons(Port);
        return server_addr;
    }


}