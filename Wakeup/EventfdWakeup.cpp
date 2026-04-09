//
// Created by harryqk on 4/7/26.
//

#include "EventfdWakeup.h"
#include "EventfdWakeup.h"
#include <sys/eventfd.h>
#include <unistd.h>
#include <errno.h>
#include <cstdlib>
#include <stdio.h>

namespace SinaiEcho
{
    EventfdWakeup::EventfdWakeup()
    {
        fd_ = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
        if (fd_ < 0)
        {
            perror("eventfd");
            abort();
        }
    }

    EventfdWakeup::~EventfdWakeup()
    {
        close(fd_);
    }

    int EventfdWakeup::GetFd() const
    {
        return fd_;
    }

    void EventfdWakeup::Wake()
    {
        uint64_t one = 1;
        write(fd_, &one, sizeof(one));
    }

    void EventfdWakeup::HandleRead()
    {
        uint64_t val;
        while (true)
        {
            ssize_t n = read(fd_, &val, sizeof(val));
            if (n <= 0)
            {
                if (errno == EAGAIN) break;
                if (errno == EINTR) continue;
                break;
            }
        }
    }
}