//
// Created by harryqk on 7/17/22.
//

#include "SCPPSocketFactoryMac.h"
#include "SocketMac.h"
#include <iostream>
namespace SinaiEcho
{
    Socket* SCPPSocketFactoryMac::CreateSocket(SocketAddressFamily AddressFamily, SocketType Type, SocketProtocol Protocol)
    {
        SocketMac* Mac = new SocketMac(AddressFamily, Type, Protocol);
        return Mac;
    }

    sockaddr_in SCPPSocketFactoryMac::CreateAddress(const char *Address, int Port)
    {
        struct sockaddr_in server_addr;
        bzero(&server_addr,sizeof(server_addr)); //Init address
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(Port);
        inet_pton(AF_INET, Address, &server_addr.sin_addr);
        return server_addr;
    }


    Socket* SCPPSocketFactoryMac::CreateConnectionSocket(int Fd)
    {
        SocketMac* Sock = new SocketMac();
        Sock->SetFileDescriptor(Fd);
        //set NonBlockMode
        bool ret = Sock->SetNonBlockMode(true);
        if(!ret)
        {

            std::printf("Set NonBlock mode fail");

        }
        return Sock;
    }

    SCPPSocketFactoryMac::SCPPSocketFactoryMac()
    {
        std::printf("construct SCPPSocketFactoryMac\n");
    }

    SCPPSocketFactoryMac::~SCPPSocketFactoryMac()
    {
        std::printf("destruct SCPPSocketFactoryMac\n");
    }




}