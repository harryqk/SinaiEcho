//
// Created by harryqk on 4/7/26.
//

#include "EpollPoller.h"
#include "../Channel.h"
#include <sys/epoll.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <cstdlib>
#include <stdio.h>

namespace SinaiEcho
{
    EpollPoller::EpollPoller()
    {
        epollFd_ = epoll_create1(EPOLL_CLOEXEC);
        if (epollFd_ < 0)
        {
            perror("epoll_create1");
            abort();
        }
    }

    EpollPoller::~EpollPoller()
    {
        close(epollFd_);
    }

    void EpollPoller::Poll(int timeoutMs, std::vector<Channel*>& activeChannels)
    {
        struct epoll_event events[64];

        int n = epoll_wait(epollFd_, events, 64, timeoutMs);

        if (n < 0)
        {
            if (errno == EINTR) return;
            perror("epoll_wait");
            return;
        }

        for (int i = 0; i < n; ++i)
        {
            Channel* ch = static_cast<Channel*>(events[i].data.ptr);

            uint32_t revents = 0;
            if (events[i].events & EPOLLIN)  revents |= Channel::kReadEvent;
            if (events[i].events & EPOLLOUT) revents |= Channel::kWriteEvent;

            ch->SetRevents(revents);
            activeChannels.push_back(ch);
        }
    }

    void EpollPoller::AddChannel(Channel* channel)
    {
        struct epoll_event ev{};
        ev.data.ptr = channel;
        ev.events = 0; // 只注册 fd，不设置事件
        epoll_ctl(epollFd_, EPOLL_CTL_ADD, channel->GetFd(), &ev);
//        struct epoll_event ev{};
//        ev.data.ptr = channel;
//
//        if (channel->IsReading()) ev.events |= EPOLLIN;
//        if (channel->IsWriting()) ev.events |= EPOLLOUT;
//
//        epoll_ctl(epollFd_, EPOLL_CTL_ADD, channel->GetFd(), &ev);
    }

    void EpollPoller::UpdateChannel(Channel* channel)
    {
        struct epoll_event ev{};
        ev.data.ptr = channel;

        if (channel->IsReading()) ev.events |= EPOLLIN;
        if (channel->IsWriting()) ev.events |= EPOLLOUT;

        epoll_ctl(epollFd_, EPOLL_CTL_MOD, channel->GetFd(), &ev);
    }

    void EpollPoller::RemoveChannel(Channel* channel)
    {
        epoll_ctl(epollFd_, EPOLL_CTL_DEL, channel->GetFd(), nullptr);
    }
}