//
// Created by harryqk on 7/17/22.
//

#include "SocketIOS.h"
#include <iostream>
#include <fcntl.h>
namespace scppsocket
{
    SocketIOS::SocketIOS(SocketAddressFamily AddressFamily, SocketType Type, SocketProtocol Protocol) : SCPPSocket(AddressFamily, Type, Protocol)
    {
        std::printf("this is SocketIOS Construct\n");
        FileDescriptor = socket(AF_INET, SOCK_STREAM, 0);
        if (FileDescriptor == SOCKET_ERROR) {
            perror("SocketIOS fail ");
        }
    }

    SocketIOS::~SocketIOS()
    {
        std::printf("this is SocketIOS Destruct\n");
    }

    int SocketIOS::Bind(int Port)
    {
        std::printf("this is IOS bind\n");

        struct sockaddr_in Address;
        memset(&Address, 0, sizeof(Address));
        Address.sin_family = AF_INET;  //
        Address.sin_addr.s_addr = INADDR_ANY;
        Address.sin_port = htons(Port);
        LocalAddress = Address;
        int ret = bind(FileDescriptor, (struct sockaddr*)&Address, sizeof(Address));
        if(ret == SOCKET_ERROR)
        {
            //printf("this is IOS Bind fail, errno=%d\n", errno);
            perror("IOS Bind fail");
        }
        else
        {
            printf("this is IOS Bind success\n");
        }
        return ret;
    }

    int SocketIOS::Listen(int MaxConnect)
    {
        printf("this is IOS Listen\n");
        int ret = listen(FileDescriptor, MaxConnect);
        if(ret < 0)
        {
            //printf("this is IOS Listen fail, errno=%d\n", errno);
            perror("IOS Listen fail");

        }
        else
        {
            printf("this is IOS Listen success\n");
        }

        return ret;
    }

    int SocketIOS::Connect(sockaddr *Address)
    {

        int ret = connect(FileDescriptor, Address, sizeof(sockaddr));
        if (ret == SOCKET_ERROR)
        {
            //std::printf("IOS Connect Error\n");
            perror("IOS Connect Error");
            //close(FileDescriptor);
        }
        else {
            std::printf("IOS Connect Success\n");
        }
        return ret;
    }

    SSocket SocketIOS::Accept(sockaddr *Address)
    {
        SockLen_t Len = sizeof(struct  sockaddr);
        SSocket NewSock = accept(FileDescriptor, Address, &Len);
        if (NewSock == SOCKET_ERROR)
        {
            //printf("IOS Accept fail, errno=%d\n", errno);
            perror("IOS Accept fail");
        }
        else {
            std::printf("IOS Accept Success\n");
        }
        return NewSock;
    }

    SockSSize_t SocketIOS::Send(const char *Buf, SockSize_t Len, int Flag)
    {
        std::printf("this is IOS Send\n");
        SockSSize_t Sent = send(FileDescriptor, Buf, Len, Flag);
        return Sent;
    }

    SockSSize_t SocketIOS::Read(char *Buf, SockSize_t Len, int Flag)
    {
        std::printf("this is IOS Recv\n");
        SockSSize_t Read = recv(FileDescriptor, Buf, Len, Flag);
        return Read;
    }

    int SocketIOS::SetNonBlockMode(bool NonBlock)
    {
        int flags = 0;

        flags = fcntl(FileDescriptor, F_GETFL, 0);
        if (SOCKET_ERROR == flags)
        {
            //printf("get sockfd flag -1, errno=%d\n", errno);
            perror("IOS SetNonBlockMode error");
            BlockMode = SocketBlockMode::Unknown;
            return BlockMode;
        }

        if (NonBlock)
        {
            if (fcntl(FileDescriptor, F_SETFL, flags | O_NONBLOCK) == SOCKET_ERROR)
            {
                //printf("set sockfd nonblock -1, errno=%d\n", errno);
                perror("IOS SetNonBlockMode error");
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
                perror("IOS SetNonBlockMode error");
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

    SocketIOS::SocketIOS()
    {
        std::printf("SocketIOS default construct\n");
    }

    bool SocketIOS::Close()
    {
        int ret;
        try {
            ret = close(FileDescriptor);
        }
        catch(std::exception e)
        {
            printf("IOS close exception=%s\n", e.what());
        }
        if(ret == SOCKET_ERROR)
        {
            //printf("IOS close errno=%d\n", errno);
            perror("IOS close error");
            return false;
        } else
        {
            printf("IOS close success\n");
            return true;
        }
    }

    bool SocketIOS::ShutDown()
    {
        int ret;
        try {
            ret = shutdown(FileDescriptor, 2);
        }
        catch(std::exception e)
        {
            printf("IOS shutdown exception=%s\n", e.what());
        }
        if(ret == SOCKET_ERROR)
        {
            //printf("IOS shut down errno=%d\n", errno);
            perror("IOS shut down error");
            return false;
        } else
        {
            printf("IOS shut down success\n");
            return true;
        }
    }

    SCPPSocket *SocketIOS::Clone(SSocket NewSocket, sockaddr_in NewPeerAddress)
    {
        SocketIOS* IOS = new SocketIOS(AddressFamily, Type, Protocol);
        IOS->SetPeerAddress(NewPeerAddress);
        IOS->SetFileDescriptor(NewSocket);
        return IOS;
    }
}
