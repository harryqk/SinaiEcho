//
// Created by harryqk on 3/30/26.
//
#include <unistd.h>
#include <iostream>
#include "../Socket.h"
#include "../SCPPSocketFactory.h"
#include "../EventLoop.h"
#include "../Channel.h"
#include "../NetConnection.h"
#include "../SocketUtil.h"
#include "../TCPClient.h"

#include <memory>
#ifdef _WIN32


#elif __APPLE__

#include "../Poller/KqueuePoller.h"
#include "../Wakeup/PipeWakeup.h"
#include "../Mac/SCPPSocketFactoryMac.h"

#elif __ANDROID__

#elif __linux__
#include "../Poller/EpollPoller.h"
#include "../Wakeup/EventfdWakeup.h"
#include "../Linux/SCPPSocketFactoryLinux.h"

#endif
using namespace std;
using namespace SinaiEcho;
int main()
{
#ifdef _WIN32


#elif __APPLE__
    KqueuePoller* poller = new KqueuePoller();
    PipeWakeup* wakeup = new PipeWakeup();


#elif __ANDROID__

#elif __linux__
    EpollPoller* poller = new EpollPoller();
    EventfdWakeup* wakeup = new EventfdWakeup();

#endif


    EventLoop loop(poller, wakeup);
    for(int i = 0; i < 1000; i++)
    {
        if(i % 30 == 0) sleep(1);
        TCPClient Client(&loop);
        Client.Connect("192.168.0.103",8888);
        //Client.Connect("192.168.0.105",8888);
    }
    loop.Loop();
}
//int main()
//{
//    Socket* Local;
//    SCPPSocketFactory* Factory;
//    Factory = new SCPPSocketFactoryMac();
//    KqueuePoller* poller = new KqueuePoller();
//    PipeWakeup* wakeup = new PipeWakeup();
//    Local = Factory->CreateSocket(SocketAddressFamily::IPv4, SocketType::SOCKTYPE_Streaming, SocketProtocol::TCP);
//    if(Local->GetFileDescriptor() == SOCKET_ERROR)
//    {
//
//        std::printf("CreateSocket Error\n");
//        return false;
//    }
//    //set NonBlockMode
//    bool ret1 = Local->SetNonBlockMode(true);
//    if(!ret1)
//    {
//
//        std::printf("Set NonBlock mode fail");
//
//        return false;
//    }
//
//
//    EventLoop loop(poller, wakeup);
//
//
//
//
//    sockaddr_in serverAddr{};
//    serverAddr.sin_family = AF_INET;
//    serverAddr.sin_port = htons(8888);
//    inet_pton(AF_INET, "192.168.0.103", &serverAddr.sin_addr);
//    int sockfd = Local->GetFileDescriptor();
//    auto Connection = std::make_shared<NetConnection>(&loop, std::unique_ptr<Socket>(Local));
//    std::weak_ptr<NetConnection> weakConn = Connection;
//    //Connection->GetChannel()->EnableReading();
//    //测试
//    Connection->SetConnectedCallback([weakConn](NetConnection*){
//        if (auto nc = weakConn.lock())   // 提升为 shared_ptr
//        {
//            std::cout << "connect success call back\n";
//            std::string msg = "this message from client";
//            //std::string big(1000000, 'A'); // 1MB
//            for (int i = 0; i < 2; i++) {
//                //std::string msg = std::to_string(i) + ":" + std::string(1000000, 'A');
//                nc->Send(msg);
//            }
//            //nc->Send(msg);               // 安全
//
//        }
//    });
//    // 发起 connect
//    int ret = connect(sockfd, (sockaddr*)&serverAddr, sizeof(serverAddr));
//
//    if (errno == EINPROGRESS)
//    {
//        Connection->SetState(kConnecting);
//        std::cout << "connect kConnecting\n";
//        Connection->GetChannel()->EnableWriting();
//        //Connection->GetChannel()->EnableReading();
//    }
//    else if(ret == 0)
//    {
//        Connection->SetState(kConnected);
//        std::cout << "connect success too fast\n";
//    }
//    else
//    {
//        std::cout << "connect error\n";
//        return 0;
//    }
//
//    // 启动事件循环
//    loop.Loop();
//
//
//
//
//    Connection.reset();
//
//    return 0;
//}