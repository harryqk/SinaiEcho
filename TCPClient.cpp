//
// Created by harryqk on 4/5/26.
//
//#include <unistd.h>
#include <iostream>
#include "TCPClient.h"
#include "SCPPSocketFactory.h"
#include "SocketUtil.h"
#include "EventLoop.h"
#include "Channel.h"

#ifdef _WIN32
#include "./Poller/SelectPoller.h"
#include "./Wakeup/SocketWakeup.h"
#include "./Win/SCPPSocketFactoryWin.h"

#elif __APPLE__

#include "./Poller/KqueuePoller.h"
#include "./Wakeup//PipeWakeup.h"
#include "./Mac/SCPPSocketFactoryMac.h"

#elif __ANDROID__

#elif __linux__
#include "./Poller/EpollPoller.h"
#include "./Wakeup//EventfdWakeup.h"
#include "./Linux/SCPPSocketFactoryLinux.h"
#include <memory>
#endif
namespace SinaiEcho
{
    TCPClient::TCPClient(EventLoop *el)
    {
#ifdef _WIN32
        Factory = new SCPPSocketFactoryWin();

#elif __APPLE__
        Factory = new SCPPSocketFactoryMac();


#elif __ANDROID__

#elif __linux__
        Factory = new SCPPSocketFactoryLinux();

#endif

        Local = Factory->CreateSocket(SocketAddressFamily::IPv4, SocketType::SOCKTYPE_Streaming, SocketProtocol::TCP);
        if(Local->GetFileDescriptor() == SOCKET_ERROR)
        {

            std::printf("CreateSocket Error\n");
            return;
        }
        //set NonBlockMode
        bool ret1 = Local->SetNonBlockMode(true);
        if(!ret1)
        {

            std::printf("Set NonBlock mode fail");

            return;
        }
        loop = el;


    }

    void TCPClient::Connect(const char* address, int port)
    {
        if(!Local)
        {
            std::cout << "Connect fail\n";
        }
        sockaddr_in serverAddr{};
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(port);
        inet_pton(AF_INET, address, &serverAddr.sin_addr);
        int sockfd = Local->GetFileDescriptor();
        //auto Connection = std::make_shared<NetConnection>(loop, std::unique_ptr<Socket>(Local));
        auto Connection = new NetConnection(loop, std::unique_ptr<Socket>(Local));


        //std::weak_ptr<NetConnection> weakConn = Connection;
        //测试
        Connection->SetConnectedCallback([this](NetConnection* nc){
            std::cout << "connect success call back\n" << std::endl;
            std::string msg = std::to_string(nc->GetFd()) + ":" + std::string("hello");
            //std::string big(1000000, 'A'); // 1MB
            for (int i = 0; i < 1; i++) {
                nc->Send(msg);
            }
        });
        // 发起 connect
        //int ret = connect(sockfd, (sockaddr*)&serverAddr, sizeof(serverAddr));
        int ret = Local->Connect((sockaddr*)&serverAddr);
        int LastError = SocketUtil::GetLastError();
        bool WouldBlock = SocketUtil::IsWouldBlock(LastError);
        bool Interrupt = SocketUtil::IsInterrupted(LastError);
        bool InProgress = SocketUtil::IsInProgress(LastError);
        if (InProgress)
        {
            Connection->SetState(kConnecting);
            std::cout << "connect kConnecting\n";
            Connection->GetChannel()->EnableWriting();
            //Connection->GetChannel()->EnableReading();
        }
        else if(ret == 0)
        {
            Connection->SetState(kConnected);
            std::cout << "connect success too fast\n";
        }
        else
        {
            std::cout << "connect error\n";
            return;
        }
    }

}