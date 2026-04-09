//
// Created by harryqk on 4/7/26.
//
#pragma once
#ifndef SINAIECHO_EVENTFDWAKEUP_H
#define SINAIECHO_EVENTFDWAKEUP_H
#include "Wakeup.h"
namespace SinaiEcho
{
    class EventfdWakeup : public Wakeup
    {
    public:
        EventfdWakeup();
        ~EventfdWakeup();

        int GetFd() const override;
        void Wake() override;
        void HandleRead() override;

    private:
        int fd_;
    };
}




#endif //SINAIECHO_EVENTFDWAKEUP_H
