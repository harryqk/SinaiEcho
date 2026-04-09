//
// Created by harryqk on 4/5/26.
//

#ifndef SINAIECHO_TCPSERVER_H
#define SINAIECHO_TCPSERVER_H
#pragma once
#include "NetConnection.h"
#include <iostream>
#include <unistd.h>
#include <unordered_map>
#include "NetMessage.h"
#include <memory>
namespace SinaiEcho
{
    class EventLoop;
    class SCPPSocketFactory;
    class TCPServer
    {
        public:
        TCPServer(int Port);

        void Start();
        void RemoveConnection(int fd);
        void Close();
        void SetMessageCallback(MessageCallback cb);
        private:
        void HandleAccept();
        int listenfd_;
        std::unordered_map<int, std::shared_ptr<NetConnection>> Connections;
        EventLoop* loop;
        Socket* Local;
        SCPPSocketFactory* Factory;

    private:
        MessageCallback OnMessage;
    };
}



#endif //SCPPSOCKET_TCPSERVER_H
