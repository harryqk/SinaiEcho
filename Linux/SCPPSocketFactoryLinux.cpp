//
// Created by harryqk on 7/17/22.
//

#include "SCPPSocketFactoryLinux.h"
#include "SocketLinux.h"

#include <iostream>
namespace SinaiEcho
{
    Socket* SCPPSocketFactoryLinux::CreateSocket(SocketAddressFamily AddressFamily, SocketType Type, SocketProtocol Protocol)
    {
        SocketLinux* Linux = new SocketLinux(AddressFamily, Type, Protocol);
        return Linux;
    }

    sockaddr_in SCPPSocketFactoryLinux::CreateAddress(const char *Address, int Port)
    {
        struct sockaddr_in server_addr;
        //bzero(&server_addr,sizeof(server_addr)); //Init address
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(Port);
        inet_pton(AF_INET, Address, &server_addr.sin_addr);
        return server_addr;
    }


    SCPPSocketFactoryLinux::SCPPSocketFactoryLinux()
    {
        std::printf("construct SCPPSocketFactoryLinux\n");
    }

    SCPPSocketFactoryLinux::~SCPPSocketFactoryLinux()
    {
        std::printf("destruct SCPPSocketFactoryLinux\n");
    }


}