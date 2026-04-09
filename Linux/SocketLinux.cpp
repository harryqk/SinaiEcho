//
// Created by harryqk on 7/17/22.
//

#include "SocketLinux.h"
#include <iostream>
#include <fcntl.h>
namespace SinaiEcho
{
    SocketLinux::SocketLinux(SocketAddressFamily AddressFamily, SocketType Type, SocketProtocol Protocol) : Socket(AddressFamily, Type, Protocol)
    {
        std::printf("this is SocketLinux Construct\n");
        FileDescriptor = socket(AF_INET, SOCK_STREAM, 0);
        if (FileDescriptor == SOCKET_ERROR) {
            perror("SocketLinux fail ");
        }
    }

    SocketLinux::~SocketLinux()
    {
        std::printf("this is SocketLinux Destruct\n");
    }

    int SocketLinux::Bind(int Port)
    {
        std::printf("this is Linux bind\n");

        struct sockaddr_in Address;
        //memset(&Address, 0, sizeof(Address));
        Address.sin_family = AF_INET;  //
        Address.sin_addr.s_addr = INADDR_ANY;
        Address.sin_port = htons(Port);
        LocalAddress = Address;
        int ret = bind(FileDescriptor, (struct sockaddr*)&Address, sizeof(Address));
        if(ret == SOCKET_ERROR)
        {
            //printf("this is mac Bind fail, errno=%d\n", errno);
            perror("Linux Bind fail");
        }
        else
        {
            printf("this is Linux Bind success\n");
        }
        return ret;
    }

    int SocketLinux::Listen(int MaxConnect)
    {
        printf("this is Linux Listen\n");
        int ret = listen(FileDescriptor, MaxConnect);
        if(ret < 0)
        {
            //printf("this is mac Listen fail, errno=%d\n", errno);
            perror("Linux Listen fail");

        }
        else
        {
            printf("this is Linux Listen success\n");
        }

        return ret;
    }

    int SocketLinux::Connect(sockaddr *Address)
    {

        int ret = connect(FileDescriptor, Address, sizeof(sockaddr));
        if (ret == SOCKET_ERROR)
        {

            perror("Linux Connect Error");
            //close(FileDescriptor);
        }
        else {
            std::printf("Linux Connect Success\n");
        }
        return ret;
    }

    SSocket SocketLinux::Accept(sockaddr *Address)
    {
        SockLen_t Len = sizeof(struct  sockaddr);
        SSocket NewSock = accept(FileDescriptor, Address, &Len);
        if (NewSock == SOCKET_ERROR)
        {
            //printf("Mac Accept fail, errno=%d\n", errno);
            perror("Linux Accept fail");
        }
        else {
            std::printf("Linux Accept Success\n");
        }
        return NewSock;
    }

    int SocketLinux::Accept(int Fd)
    {
        int NewSock = accept(Fd, nullptr, nullptr);
        if (NewSock == SOCKET_ERROR)
        {
            perror("Mac Accept Fail Error:");
        }
        else {
            std::printf("Mac Accept Success\n");
        }
        return NewSock;
    }

    SockSSize_t SocketLinux::Send(const char *Buf, SockSize_t Len, int Flag)
    {
        std::printf("this is Linux Send\n");
        SockSSize_t Sent = send(FileDescriptor, Buf, Len, Flag);
        return Sent;
    }

    SockSSize_t SocketLinux::Read(char *Buf, SockSize_t Len, int Flag)
    {
        std::printf("this is Linux Recv\n");
        SockSSize_t Read = recv(FileDescriptor, Buf, Len, Flag);
        return Read;
    }

    int SocketLinux::SetNonBlockMode(bool NonBlock)
    {
        int flags = 0;

        flags = fcntl(FileDescriptor, F_GETFL, 0);
        if (SOCKET_ERROR == flags)
        {
            //printf("get sockfd flag -1, errno=%d\n", errno);
            perror("Linux SetNonBlockMode error");
            BlockMode = SocketBlockMode::Unknown;
            return BlockMode;
        }

        if (NonBlock)
        {
            if (fcntl(FileDescriptor, F_SETFL, flags | O_NONBLOCK) == SOCKET_ERROR)
            {
                //printf("set sockfd nonblock -1, errno=%d\n", errno);
                perror("Linux SetNonBlockMode error");
                BlockMode = SocketBlockMode::Unknown;
                return BlockMode;
            }
            else
            {
                BlockMode = SocketBlockMode::NonBlock;
            }
        }
        else
        {
            if (fcntl(FileDescriptor, F_SETFL, flags & (~O_NONBLOCK)) == SOCKET_ERROR)
            {
                //printf("set sockfd nonblock -1, errno=%d\n", errno);
                perror("Linux SetNonBlockMode error");
                BlockMode = SocketBlockMode::Unknown;
                return BlockMode;
            }
            else
            {
                BlockMode = SocketBlockMode::Block;
            }
        }

        return BlockMode;


    }

    SocketLinux::SocketLinux()
    {
        std::printf("SCPPSocketMac default construct\n");
    }

    bool SocketLinux::Close()
    {
        int ret;
        try {
            ret = close(FileDescriptor);
        }
        catch(std::exception e)
        {
            printf("mac close exception=%s\n", e.what());
        }
        if(ret == SOCKET_ERROR)
        {
            //printf("mac close errno=%d\n", errno);
            perror("Mac close error");
            return false;
        } else
        {
            printf("mac close success\n");
            return true;
        }
    }

    bool SocketLinux::ShutDown()
    {
        int ret;
        try {
            ret = shutdown(FileDescriptor, 2);
        }
        catch(std::exception e)
        {
            printf("mac shutdown exception=%s\n", e.what());
        }
        if(ret == SOCKET_ERROR)
        {
            //printf("mac shut down errno=%d\n", errno);
            perror("Mac shut down error");
            return false;
        } else
        {
            printf("mac shut down success\n");
            return true;
        }
    }

    Socket* SocketLinux::Clone(SSocket NewSocket, sockaddr_in NewPeerAddress)
    {
        SocketLinux* Mac = new SocketLinux(AddressFamily, Type, Protocol);
        Mac->SetPeerAddress(NewPeerAddress);
        Mac->SetFileDescriptor(NewSocket);
        return Mac;
    }

    bool SocketLinux::GetSocketError(int& err)
    {
        socklen_t len = sizeof(err);
        return getsockopt(FileDescriptor, SOL_SOCKET, SO_ERROR, &err, &len) == 0;
    }
}
