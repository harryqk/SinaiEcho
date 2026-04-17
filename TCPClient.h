//
// Created by harryqk on 4/5/26.
//

#ifndef SINAIECHO_TCPCLIENT_H
#define SINAIECHO_TCPCLIENT_H
#pragma once
#include "NetConnection.h"
#include <iostream>
//#include <unistd.h>
namespace SinaiEcho
{
    class EventLoop;
    class SCPPSocketFactory;
    class Socket;
    class TCPClient
    {
    public:
        TCPClient(EventLoop* el);

        void Connect(const char* address, int port);
    private:
        EventLoop* loop;
        Socket* Local;
        SCPPSocketFactory* Factory;
    };
}




#endif //SINAIECHO_TCPCLIENT_H
