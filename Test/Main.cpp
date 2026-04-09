//
// Created by harryqk on 3/28/26.
//

#include "../EventLoop.h"
#include "../NetConnection.h"
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include "../Channel.h"
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include "../Socket.h"
//#include "../SCPPSocketFactory.h"
//#include "../Mac/SCPPSocketFactoryMac.h"
//#include "../Poller/KqueuePoller.h"
//#include "../Wakeup/PipeWakeup.h"
#include "../TCPServer.h"

using namespace SinaiEcho;
void OnMessage(const NetMessage &msg)
{
    std::cout << "recv from connection fd:=" << msg.fd << " " << msg.Data << std::endl;
}

int main()
{
    TCPServer Server(8888);
    Server.SetMessageCallback(OnMessage);
    Server.Start();
    return 0;
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
//    bool ret = Local->SetNonBlockMode(true);
//    if(!ret)
//    {
//
//        std::printf("Set NonBlock mode fail");
//
//        return false;
//    }
//    //bind
//    int result;
//    result = Local->Bind(8888);
//    if(result == SOCKET_ERROR)
//    {
//        std::printf("Bind fail");
//
//        return false;
//    }
//    //listen;
//    result = Local->Listen(4);
//    if(result == SOCKET_ERROR)
//    {
//        std::printf("Listen fail");
//        return false;
//    }
//
//    EventLoop loop(poller, wakeup);
//    int listenFd = Local->GetFileDescriptor();
//    // 👉 用 Channel 管 listenfd
//    Channel* acceptChannel = new Channel(&loop, listenFd);
//    loop.AddChannel(acceptChannel);
//    acceptChannel->EnableReading();
//    acceptChannel->SetReadCallback([&]() {
//        int connfd = Local->Accept(listenFd);
//        Socket* Client = Factory->CreateConnectionSocket(connfd);
//        //int connfd = accept(listenFd, nullptr, nullptr);
//
//        if (connfd < 0)
//        {
//            // 非阻塞 accept 没连接了
//            std::cout << "非阻塞 accept 没连接了" << connfd << std::endl;
//        }
//        else
//        {
//            std::cout << "new connection fd:=" << connfd << std::endl;
//
//            // 👉 交给你的 Connection
//            NetConnection* Connection = new NetConnection(&loop, std::unique_ptr<Socket>(Client));
//            Connection->SetState(kConnected);
//            Connection->GetChannel()->EnableReading();
//            Connection->GetChannel()->EnableWriting();
//        }
//
//    });
//
//
//    std::cout << "server start at 8888...\n";
//
//    loop.Loop();
//    acceptChannel->DisableWriting();
//    acceptChannel->DisableReading();
//    delete acceptChannel;
//    return 0;
//}