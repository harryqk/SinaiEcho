//
// Created by harryqk on 7/17/22.
//

#include "Socket.h"

namespace SinaiEcho
{

    Socket::Socket()
    {
        BlockMode = SocketBlockMode::Block;
    }



    SSocket Socket::GetFileDescriptor() const
    {
        return FileDescriptor;
    }

    void Socket::SetFileDescriptor(SSocket fileDescriptor)
    {
        FileDescriptor = fileDescriptor;
    }



    Socket::Socket(SocketAddressFamily AddressFamily, SocketType Type, SocketProtocol Protocol)
    {
        BlockMode = SocketBlockMode::Block;
    }

    Socket::~Socket()
    {

    }




}