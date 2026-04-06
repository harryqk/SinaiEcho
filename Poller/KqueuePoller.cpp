//
// Created by harryqk on 3/27/26.
//

#include "KqueuePoller.h"
#include "../Channel.h"

#include <sys/event.h>
#include <unistd.h>
#include <iostream>
namespace SinaiEcho
{
    // ================= 构造 / 析构 =================
    KqueuePoller::KqueuePoller()
    {
        kqueueFd_ = kqueue();
    }

    KqueuePoller::~KqueuePoller()
    {
        close(kqueueFd_);
    }

// ================= Poll =================
    void KqueuePoller::Poll(int timeoutMs, std::vector<Channel*>& activeChannels)
    {
        struct kevent events[64];

        struct timespec ts;
        ts.tv_sec = timeoutMs / 1000;
        ts.tv_nsec = (timeoutMs % 1000) * 1000000;

        //int n = kevent(kqueueFd_, nullptr, 0, events, 64, &ts);
        int n = kevent(kqueueFd_, nullptr, 0, events, 64, nullptr);

        if (n < 0)
        {
            if (errno == EINTR)
            {
                // 被编译器断点等信号打断，继续等
                std::cout << "signal break keep wait=" << std::endl;
            }
            else
            {
                perror("kevent error");
            }
        }
        for (int i = 0; i < n; ++i)
        {
            Channel* ch = static_cast<Channel*>(events[i].udata);

            uint32_t revents = 0;

            if (events[i].filter == EVFILT_READ)
                revents |= Channel::kReadEvent;

            if (events[i].filter == EVFILT_WRITE)
                revents |= Channel::kWriteEvent;

            // 把revents设置回Channel（关键）
            ch->SetRevents(revents);

            // 交给EventLoop处理，而不是在这里Handle
            activeChannels.push_back(ch);
        }
    }

// ================= Add =================
    void KqueuePoller::AddChannel(Channel* channel)
    {
        int fd = channel->GetFd();

        struct kevent ev[2];
        int n = 0;
        EV_SET(&ev[0], fd, EVFILT_READ, EV_ADD, 0, 0, channel);
        EV_SET(&ev[1], fd, EVFILT_WRITE, EV_ADD, 0, 0, channel);
        // 用想要注册的事件，而不是 revents
//    if (channel->IsReading())
//    {
//        EV_SET(&ev[n++], fd, EVFILT_READ, EV_ADD, 0, 0, channel);
//    }
//
//    if (channel->IsWriting())
//    {
//        EV_SET(&ev[n++], fd, EVFILT_WRITE, EV_ADD, 0, 0, channel);
//    }
        kevent(kqueueFd_, ev, 2, nullptr, 0, nullptr);
    }

// ================= Update =================
    void KqueuePoller::UpdateChannel(Channel* channel)
    {
        int fd = channel->GetFd();
        struct kevent ev[2];
        int n = 0;

        if (channel->IsReading())
        {
            EV_SET(&ev[n++], fd, EVFILT_READ,  EV_ENABLE, 0, 0, channel);
        }
        else
        {
            EV_SET(&ev[n++], fd, EVFILT_READ, EV_DISABLE, 0, 0, channel);
        }

        if (channel->IsWriting())
        {
            EV_SET(&ev[n++], fd, EVFILT_WRITE,  EV_ENABLE, 0, 0, channel);
        }
        else
        {
            EV_SET(&ev[n++], fd, EVFILT_WRITE, EV_DISABLE, 0, 0, channel);
        }

        if (n > 0)
        {
            kevent(kqueueFd_, ev, n, nullptr, 0, nullptr);
        }
    }

// ================= Remove =================
    void KqueuePoller::RemoveChannel(Channel* channel)
    {
        int fd = channel->GetFd();

        struct kevent ev[2];

        EV_SET(&ev[0], fd, EVFILT_READ, EV_DELETE, 0, 0, nullptr);
        EV_SET(&ev[1], fd, EVFILT_WRITE, EV_DELETE, 0, 0, nullptr);

        kevent(kqueueFd_, ev, 2, nullptr, 0, nullptr);
    }
}
