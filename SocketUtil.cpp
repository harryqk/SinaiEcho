//
// Created by harryqk on 7/22/22.
//

#include "SocketUtil.h"
#include <iostream>
#include <cstring>
#ifdef _WIN32
#include <WinSock2.h>
#else
#include <errno.h>
#endif
namespace SinaiEcho
{

    void SocketUtil::IntToByte(int i, unsigned char*bytes, int size)
    {
        memset(bytes,0,sizeof(unsigned char) *  size);
        bytes[0] = (unsigned char) (0xff & i);
        bytes[1] = (unsigned char) ((0xff00 & i) >> 8);
        bytes[2] = (unsigned char) ((0xff0000 & i) >> 16);
        bytes[3] = (unsigned char) ((0xff000000 & i) >> 24);
    }

    int SocketUtil::BytesToInt(unsigned char*bytes, int size)
    {
        int ret = bytes[0] & 0xFF;
        ret |= ((bytes[1] << 8) & 0xFF00);
        ret |= ((bytes[2] << 16) & 0xFF0000);
        ret |= ((bytes[3] << 24) & 0xFF000000);
        return ret;
    }

    int SocketUtil::GetLastError()
    {
#ifdef _WIN32
        return WSAGetLastError();
#else
        return errno;
#endif
    }
    bool SocketUtil::IsWouldBlock(int err)
    {
#ifdef _WIN32
        return err == WSAEWOULDBLOCK;
#else
        return err == EAGAIN || err == EWOULDBLOCK;
#endif
    }
    bool SocketUtil::IsInterrupted(int err)
    {
#ifdef _WIN32
        return err == WSAEINTR;
#else
        return err == EINTR;
#endif
    }

    bool SocketUtil::IsInProgress(int err)
    {
#ifdef _WIN32
        return err == WSAEWOULDBLOCK || err == WSAEINPROGRESS;
#else
        return err == EINPROGRESS;
#endif
    }
}