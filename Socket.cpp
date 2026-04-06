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

    void Socket::SetConnectionState(SocketConnectionState connectionState)
    {
        ConnectionState = connectionState;
    }

    SocketBlockMode Socket::GetBlockMode() const
    {
        return BlockMode;
    }

    SSocket Socket::GetFileDescriptor() const
    {
        return FileDescriptor;
    }

    void Socket::SetFileDescriptor(SSocket fileDescriptor)
    {
        FileDescriptor = fileDescriptor;
    }

    const sockaddr_in &Socket::GetLocalAddress() const
    {
        return LocalAddress;
    }

    void Socket::SetLocalAddress(const sockaddr_in &localAddress)
    {
        LocalAddress = localAddress;
    }

    const sockaddr_in &Socket::GetPeerAddress() const
    {
        return PeerAddress;
    }

    void Socket::SetPeerAddress(const sockaddr_in &peerAddress)
    {
        PeerAddress = peerAddress;
    }

    Socket::Socket(SocketAddressFamily AddressFamily, SocketType Type, SocketProtocol Protocol)
    {
        BlockMode = SocketBlockMode::Block;
    }

    Socket::~Socket()
    {

    }

    SocketConnectionState Socket::GetConnectionState()
    {
        return ConnectionState;
    }


}