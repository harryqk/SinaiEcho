//
// Created by harryqk on 3/27/26.
//

#include "Channel.h"
#include "EventLoop.h"
namespace SinaiEcho
{
    Channel::Channel(EventLoop* loop, int fd)
            : Loop(loop), fd(fd)
    {
    }

    Channel::~Channel() {}

    void Channel::HandleEvent()
    {
        if ((REvents_ & kReadEvent) && ReadCallback)
            ReadCallback();

        if ((REvents_ & kWriteEvent) && WriteCallback)
            WriteCallback();
    }

    void Channel::EnableReading()
    {
        Events_ |= kReadEvent;
        Loop->UpdateChannel(this);
    }

    void Channel::EnableWriting()
    {
        Events_ |= kWriteEvent;
        Loop->UpdateChannel(this);
    }

    void Channel::DisableWriting()
    {
        Events_ &= ~kWriteEvent;
        Loop->UpdateChannel(this);
    }

    void Channel::DisableReading()
    {
        Events_ &= ~kReadEvent;
        Loop->UpdateChannel(this);
    }

    void Channel::SetRevents(uint32_t revents)
    {
        REvents_ = revents;
    }

    uint32_t Channel::GetRevents() const
    {
        return REvents_;
    }
}
