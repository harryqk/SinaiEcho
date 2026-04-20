//
// Created by harryqk on 3/27/26.
//

#include "EventLoop.h"
#include "Channel.h"
#include "./Wakeup/Wakeup.h"
#include "./Poller/Poller.h"
//#include <unistd.h>
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


        WakeupChannel = new Channel(this, wakeup->GetFd());

        WakeupChannel->SetReadCallback([this] {
            HandleWakeup();
        });

        WakeupChannel->EnableReading();
    }

    EventLoop::~EventLoop()
    {
        WakeupChannel->DisableReading();
        WakeupChannel->DisableWriting();
        delete WakeupChannel;
    }

    void EventLoop::Loop()
    {
        while (!Quit)
        {
            ActiveChannels.clear();

            poller->Poll(1000, ActiveChannels);

            for (auto ch : ActiveChannels)
            {
                ch->HandleEvent();
            }

            // 执行任务
            std::queue<std::function<void()>> tasks;
            {
                std::lock_guard<std::mutex> lock(Mutex);
                std::swap(tasks, PendingTasks);
            }

            while (!tasks.empty())
            {
                tasks.front()();
                tasks.pop();
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
            std::lock_guard<std::mutex> lock(Mutex);
            PendingTasks.push(cb);
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



