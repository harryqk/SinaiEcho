//
// Created by harryqk on 3/27/26.
//

#include "EventLoop.h"
#include "Channel.h"
#include "./Wakeup/PipeWakeup.h"
#include "./Poller/KqueuePoller.h"
#include <unistd.h>
namespace SinaiEcho
{
    EventLoop::EventLoop()
    {
//#ifdef _WIN32
//
//#elif __APPLE__
//    poller = new KqueuePoller();
//    wakeup = new PipeWakeup();
//#elif __ANDROID__
//#elif __linux__
//#endif
//
//
//    wakeupChannel = new Channel(this, wakeup->GetFd());
//
//    wakeupChannel->SetReadCallback([this] {
//        HandleWakeup();
//    });
//
//    wakeupChannel->EnableReading();
    }

    EventLoop::EventLoop(Poller *p, Wakeup *w)
    {

        poller = p;
        wakeup = w;


        wakeupChannel = new Channel(this, wakeup->GetFd());

        wakeupChannel->SetReadCallback([this] {
            HandleWakeup();
        });

        wakeupChannel->EnableReading();
    }

    EventLoop::~EventLoop()
    {
        wakeupChannel->DisableReading();
        wakeupChannel->DisableWriting();
        delete wakeupChannel;
    }

    void EventLoop::Loop()
    {
        while (!quit)
        {
            activeChannels.clear();

            poller->Poll(1000, activeChannels);

            for (auto ch : activeChannels)
            {
                ch->HandleEvent();
            }

            // 执行任务
            std::queue<std::function<void()>> functors;
            {
                std::lock_guard<std::mutex> lock(mutex);
                std::swap(functors, pendingFunctors);
            }

            while (!functors.empty())
            {
                functors.front()();
                functors.pop();
            }
        }
    }

    void EventLoop::UpdateChannel(Channel* channel)
    {
        poller->UpdateChannel(channel);
    }

    void EventLoop::RunInLoop(std::function<void()> cb)
    {
        {
            std::lock_guard<std::mutex> lock(mutex);
            pendingFunctors.push(cb);
        }

        wakeup->Wake();
    }

    void EventLoop::AddChannel(Channel *ch)
    {
        poller->AddChannel(ch);
    }

    void EventLoop::HandleWakeup()
    {
        wakeup->HandleRead();
//        char buf[64];
//        read(wakeup->GetFd(), buf, sizeof(buf));
    }

    void EventLoop::RemoveChannel(Channel *channel)
    {
        poller->RemoveChannel(channel);
    }
}



