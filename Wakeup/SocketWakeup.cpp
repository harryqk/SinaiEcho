#include "SocketWakeup.h"
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

namespace SinaiEcho
{
    static void CreateSocketPair(SOCKET& readSock, SOCKET& writeSock)
    {
        SOCKET listener = socket(AF_INET, SOCK_STREAM, 0);

        sockaddr_in addr{};
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
        addr.sin_port = 0;

        bind(listener, (sockaddr*)&addr, sizeof(addr));
        listen(listener, 1);

        int len = sizeof(addr);
        getsockname(listener, (sockaddr*)&addr, &len);

        writeSock = socket(AF_INET, SOCK_STREAM, 0);
        connect(writeSock, (sockaddr*)&addr, sizeof(addr));

        readSock = accept(listener, nullptr, nullptr);

        closesocket(listener);
    }

    SocketWakeup::SocketWakeup()
    {
        CreateSocketPair(readSock_, writeSock_);

        u_long mode = 1;
        ioctlsocket(readSock_, FIONBIO, &mode);
        ioctlsocket(writeSock_, FIONBIO, &mode);
    }

    SocketWakeup::~SocketWakeup()
    {
        closesocket(readSock_);
        closesocket(writeSock_);
    }

    int SocketWakeup::GetFd() const
    {
        return (int)readSock_;
    }

    void SocketWakeup::Wake()
    {
        char one = 1;
        send(writeSock_, &one, 1, 0);
    }

    void SocketWakeup::HandleRead()
    {
        char buf[64];
        while (true)
        {
            int n = recv(readSock_, buf, sizeof(buf), 0);
            if (n <= 0)
            {
                int err = WSAGetLastError();
                if (err == WSAEWOULDBLOCK)
                    break;
                break;
            }
        }
    }
}