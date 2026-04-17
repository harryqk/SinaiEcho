//
// Created by pc on 2026/4/12.
//

#ifndef SINAIECHO_SOCKETWAKEUP_H
#define SINAIECHO_SOCKETWAKEUP_H

#pragma once

#include "Wakeup.h"
#include <winsock2.h>

namespace SinaiEcho
{
    class SocketWakeup : public Wakeup
    {
    public:
        SocketWakeup();
        ~SocketWakeup();

        int GetFd() const override;
        void Wake() override;
        void HandleRead() override;

    private:
        SOCKET readSock_;
        SOCKET writeSock_;
    };
}
#endif //SINAIECHO_SOCKETWAKEUP_H
