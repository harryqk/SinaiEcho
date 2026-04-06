//
// Created by harryqk on 7/17/22.
//

#include "SCPPSocketFactoryAndroid.h"
#include "SCPPSocketAndroid.h"

#include <iostream>
namespace SinaiEcho
{
    Socket* SCPPSocketFactoryAndroid::CreateSocket(SocketAddressFamily AddressFamily, SocketType Type, SocketProtocol Protocol)
    {
        SocketAndroid* Android = new SocketAndroid(AddressFamily, Type, Protocol);
        return Android;
    }

    sockaddr_in SCPPSocketFactoryAndroid::CreateAddress(const char *Address, int Port)
    {
        struct sockaddr_in server_addr;
        //bzero(&server_addr,sizeof(server_addr)); //Init address
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(Port);
        inet_pton(AF_INET, Address, &server_addr.sin_addr);
        return server_addr;
    }
    

    SCPPSocketFactoryAndroid::SCPPSocketFactoryAndroid()
    {
        std::printf("construct SCPPSocketFactoryAndroid\n");
    }

    SCPPSocketFactoryAndroid::~SCPPSocketFactoryAndroid()
    {
        std::printf("destruct SCPPSocketFactoryAndroid\n");
    }


}