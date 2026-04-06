//
// Created by harryqk on 3/27/26.
//

#ifndef SINAIECHO_KQUEUEPOLLER_H
#define SINAIECHO_KQUEUEPOLLER_H


#pragma once
#include "Poller.h"
#include <sys/event.h>
#include <vector>
namespace SinaiEcho
{
    class KqueuePoller : public Poller
    {
    public:
        KqueuePoller();
        ~KqueuePoller();

        void Poll(int timeoutMs, std::vector<Channel*>& activeChannels) override;

        void AddChannel(Channel* channel) override;
        void UpdateChannel(Channel* channel) override;
        void RemoveChannel(Channel* channel) override;

    private:
        int kqueueFd_;
    };
}

#endif //SINAIECHO_KQUEUEPOLLER_H
