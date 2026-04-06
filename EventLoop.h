//
// Created by harryqk on 3/27/26.
//

#ifndef SINAIECHO_EVENTLOOP_H
#define SINAIECHO_EVENTLOOP_H


#pragma once
#include <vector>
#include <queue>
#include <functional>
#include <mutex>
namespace SinaiEcho
{
    class Channel;
    class Poller;
    class Wakeup;

    class EventLoop
    {
    public:
        EventLoop();
        EventLoop(Poller* p, Wakeup* w);
        ~EventLoop();

        void Loop();
        void UpdateChannel(Channel* channel);

        void RunInLoop(std::function<void()> cb);
        void AddChannel(Channel* ch);
        void RemoveChannel(Channel* channel);


    private:
        void HandleWakeup();

    private:
        bool Quit = false;

        Poller* poller;
        std::vector<Channel*> ActiveChannels;

        std::queue<std::function<void()>> PendingFunctors;
        std::mutex Mutex;

        Wakeup* wakeup;
        Channel* WakeupChannel;
    };
}


#endif //SINAIECHO_EVENTLOOP_H
