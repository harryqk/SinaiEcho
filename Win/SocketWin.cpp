//
// Created by harryqk on 7/17/22.
//

#include "SocketWin.h"
#include <iostream>
namespace SinaiEcho
{
    SocketWin::SocketWin(SocketAddressFamily AddressFamily, SocketType Type, SocketProtocol Protocol) : Socket(AddressFamily, Type, Protocol)
    {
        std::printf("this is SocketWin Construct\n");

        //----------------------
        // Create a SOCKET for connecting to server
        FileDescriptor = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (FileDescriptor == INVALID_SOCKET) {
            wprintf(L"socket function failed with error: %ld\n", WSAGetLastError());
        }
    }

    SocketWin::~SocketWin()
    {
       //WSACleanup();
        std::printf("this is SocketWin Destruct\n");
    }

    int SocketWin::Bind(int Port)
    {
        std::printf("this is Win bind\n");

        struct sockaddr_in Address;
        memset(&Address, 0, sizeof(Address));
        Address.sin_family = AF_INET;  //
        Address.sin_addr.s_addr = INADDR_ANY;
        Address.sin_port = htons(Port);
        LocalAddress = Address;
        int ret = bind(FileDescriptor, (struct sockaddr*)&Address, sizeof(Address));
        if(ret == SOCKET_ERROR)
        {
            //printf("this is mac Bind fail, errno=%d\n", errno);
            perror("Win Bind fail");
        }
        else
        {
            printf("this is Win Bind success\n");
        }
        return ret;
    }

    int SocketWin::Listen(int MaxConnect)
    {
        printf("this is Win Listen\n");
        int ret = listen(FileDescriptor, MaxConnect);
        if(ret < 0)
        {
            //printf("this is mac Listen fail, errno=%d\n", errno);
            perror("Win Listen fail");

        }
        else
        {
            printf("this is Win Listen success\n");
        }

        return ret;
    }

    int SocketWin::Connect(sockaddr *Address)
    {
        //----------------------
        // Connect to server.
        int ret = connect(FileDescriptor, Address, sizeof (sockaddr));
        if (ret == SOCKET_ERROR) {
            ret = WSAGetLastError();
            wprintf(L"connect function failed with error: %ld\n", ret);
        }
        else {
            printf("Win Connect Success\n");
        }
        return ret;
    }

    SSocket SocketWin::Accept(sockaddr *Address)
    {
        SockLen_t Len = sizeof(struct  sockaddr);
        SSocket NewSock = accept(FileDescriptor, Address, &Len);
        if (NewSock == SOCKET_ERROR)
        {
            printf("Win Accept fail, errno=%d\n", errno);
            perror("Win Accept fail");
            //WSACleanup();
            return 0;
        }
        else {
            std::printf("Win Accept Success\n");
        }
        return NewSock;
    }

    int SocketWin::Accept(int Fd)
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

    SockSSize_t SocketWin::Send(const char *Buf, SockSize_t Len, int Flag)
    {
        std::printf("this is Win Send\n");
        SockSSize_t Sent = send(FileDescriptor, Buf, Len, Flag);
        return Sent;
    }

    SockSSize_t SocketWin::Read(char *Buf, SockSize_t Len, int Flag)
    {
        std::printf("this is Win Recv\n");
        SockSSize_t Read = recv(FileDescriptor, Buf, Len, Flag);
        return Read;
    }

    int SocketWin::SetNonBlockMode(bool NonBlock)
    {
        int flags = 0;
        int iMode = 1; //
        if (NonBlock)
        {
            flags =  ioctlsocket(FileDescriptor, FIONBIO, (u_long FAR*)&iMode);
            if (flags == SOCKET_ERROR)
            {
                //printf("set sockfd nonblock -1, errno=%d\n", errno);
                perror("win SetNonBlockMode error");
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
            iMode = 0;
            flags =  ioctlsocket((SOCKET)socket, FIONBIO, (u_long FAR*)&iMode);
            if (flags == SOCKET_ERROR)
            {
                //printf("set sockfd nonblock -1, errno=%d\n", errno);
                perror("win SetNonBlockMode error");
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

    SocketWin::SocketWin()
    {
        std::printf("SocketWin default construct\n");
    }

    bool SocketWin::Close()
    {
        int ret;
        try {
            ret = closesocket(FileDescriptor);
        }
        catch(std::exception e)
        {
            printf("Win close exception=%s\n", e.what());
        }
        if(ret == SOCKET_ERROR)
        {
            //printf("mac close errno=%d\n", errno);
            perror("Win close error");
            return false;
        } else
        {
            printf("Win close success\n");
            return true;
        }
    }

    bool SocketWin::ShutDown()
    {
        int ret;
        try {
            ret = shutdown(FileDescriptor, 2);
        }
        catch(std::exception e)
        {
            printf("Win shutdown exception=%s\n", e.what());
        }
        if(ret == SOCKET_ERROR)
        {
            //printf("mac shut down errno=%d\n", errno);
            perror("Win shut down error");
            return false;
        } else
        {
            printf("Win shut down success\n");
            return true;
        }
    }

    Socket *SocketWin::Clone(SSocket NewSocket, sockaddr_in NewPeerAddress)
    {
        SocketWin* Win = new SocketWin(AddressFamily, Type, Protocol);
        Win->SetPeerAddress(NewPeerAddress);
        Win->SetFileDescriptor(NewSocket);
        return Win;
    }

    bool SocketWin::GetSocketError(int& err)
    {
        socklen_t len = sizeof(err);
        return getsockopt(FileDescriptor, SOL_SOCKET, SO_ERROR, (char*)&err, &len) == 0;
    }

}
