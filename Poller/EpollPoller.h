//
// Created by harryqk on 4/7/26.
//

#ifndef SINAIECHO_EPOLLPOLLER_H
#define SINAIECHO_EPOLLPOLLER_H
#pragma once
#include "Poller.h"

namespace SinaiEcho
{
    class EpollPoller : public Poller
    {
    public:
        EpollPoller();
        ~EpollPoller();

        void Poll(int timeoutMs, std::vector<Channel*>& activeChannels) override;

        void AddChannel(Channel* channel) override;
        void UpdateChannel(Channel* channel) override;
        void RemoveChannel(Channel* channel) override;

    private:
        int epollFd_;
    };
}


#endif //SINAIECHO_EPOLLPOLLER_H
