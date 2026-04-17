#include "SelectPoller.h"
#include "../Channel.h"
#include <algorithm>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

namespace SinaiEcho
{
    SelectPoller::SelectPoller()
    {
        //WSADATA wsa;
       // WSAStartup(MAKEWORD(2, 2), &wsa);
    }

    SelectPoller::~SelectPoller()
    {
       // WSACleanup();
    }

    void SelectPoller::Poll(int timeoutMs, std::vector<Channel*>& activeChannels)
    {
        fd_set readSet;
        fd_set writeSet;

        FD_ZERO(&readSet);
        FD_ZERO(&writeSet);

        for (auto ch : channels_)
        {
            SOCKET fd = (SOCKET)ch->GetFd();

            if (ch->IsReading())
                FD_SET(fd, &readSet);

            if (ch->IsWriting())
                FD_SET(fd, &writeSet);
        }

        timeval tv;
        tv.tv_sec = timeoutMs / 1000;
        tv.tv_usec = (timeoutMs % 1000) * 1000;

        int n = select(0, &readSet, &writeSet, nullptr, &tv);
        if (n <= 0) return;

        for (auto ch : channels_)
        {
            SOCKET fd = (SOCKET)ch->GetFd();
            uint32_t revents = 0;

            if (FD_ISSET(fd, &readSet))
                revents |= Channel::kReadEvent;

            if (FD_ISSET(fd, &writeSet))
                revents |= Channel::kWriteEvent;

            if (revents)
            {
                ch->SetRevents(revents);
                activeChannels.push_back(ch);
            }
        }
    }

    void SelectPoller::AddChannel(Channel* channel)
    {
        channels_.push_back(channel);
    }

    void SelectPoller::UpdateChannel(Channel* channel)
    {
        // select 不需要更新
    }

    void SelectPoller::RemoveChannel(Channel* channel)
    {
        channels_.erase(
            std::remove(channels_.begin(), channels_.end(), channel),
            channels_.end()
        );
    }
}