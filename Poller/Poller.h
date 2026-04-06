//
// Created by harryqk on 3/27/26.
//

#ifndef SINAIECHO_POLLER_H
#define SINAIECHO_POLLER_H


#pragma once
#include <vector>
namespace SinaiEcho
{
    class Channel;

    class Poller
    {
    public:
        virtual ~Poller() {}

        virtual void Poll(int timeoutMs, std::vector<Channel*>& activeChannels) = 0;
        virtual void UpdateChannel(Channel* channel) = 0;
        virtual void AddChannel(Channel* ch) = 0;
        // 删除 fd
        virtual void RemoveChannel(Channel* channel) = 0;
    };

}

#endif //SINAIECHO_POLLER_H
