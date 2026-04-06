//
// Created by harryqk on 3/27/26.
//

#include "Channel.h"
#include "EventLoop.h"
namespace SinaiEcho
{
    Channel::Channel(EventLoop* loop, int fd)
            : loop(loop), fd(fd)
    {
    }

    Channel::~Channel() {}

    void Channel::HandleEvent()
    {
        if ((revents_ & kReadEvent) && readCallback)
            readCallback();

        if ((revents_ & kWriteEvent) && writeCallback)
            writeCallback();
    }

    void Channel::EnableReading()
    {
        events_ |= kReadEvent;
        loop->UpdateChannel(this);
    }

    void Channel::EnableWriting()
    {
        events_ |= kWriteEvent;
        loop->UpdateChannel(this);
    }

    void Channel::DisableWriting()
    {
        events_ &= ~kWriteEvent;
        loop->UpdateChannel(this);
    }

    void Channel::DisableReading()
    {
        events_ &= ~kReadEvent;
        loop->UpdateChannel(this);
    }

    void Channel::SetRevents(uint32_t revents)
    {
        revents_ = revents;
    }

    uint32_t Channel::GetRevents() const
    {
        return revents_;
    }
}
