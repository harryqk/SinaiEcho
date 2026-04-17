//
// Created by pc on 2026/4/12.
//

#ifndef SINAIECHO_SELECTPOLLER_H
#define SINAIECHO_SELECTPOLLER_H

#pragma once

#include "Poller.h"
#include <vector>
#include <winsock2.h>

namespace SinaiEcho
{
    class SelectPoller : public Poller
    {
    public:
        SelectPoller();
        ~SelectPoller();

        void Poll(int timeoutMs, std::vector<Channel*>& activeChannels) override;

        void AddChannel(Channel* channel) override;
        void UpdateChannel(Channel* channel) override;
        void RemoveChannel(Channel* channel) override;

    private:
        std::vector<Channel*> channels_;
    };
}

#endif //SINAIECHO_SELECTPOLLER_H
