//
// Created by harryqk on 7/17/22.
//

#include "SCPPSocketFactoryIOS.h"
#include "SocketIOS.h"

#include <iostream>
namespace SinaiEcho
{
    Socket* SCPPSocketFactoryIOS::CreateSocket(SocketAddressFamily AddressFamily, SocketType Type, SocketProtocol Protocol)
    {
        SocketIOS* IOS = new SocketIOS(AddressFamily, Type, Protocol);
        return IOS;
    }

    sockaddr_in SCPPSocketFactoryIOS::CreateAddress(const char *Address, int Port)
    {
        struct sockaddr_in server_addr;
        bzero(&server_addr,sizeof(server_addr)); //Init address
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(Port);
        inet_pton(AF_INET, Address, &server_addr.sin_addr);
        return server_addr;
    }



    SCPPSocketFactoryIOS::SCPPSocketFactoryIOS()
    {
        std::printf("construct SCPPSocketFactoryIOS\n");
    }

    SCPPSocketFactoryIOS::~SCPPSocketFactoryIOS()
    {
        std::printf("destruct SCPPSocketFactoryIOS\n");
    }


}
