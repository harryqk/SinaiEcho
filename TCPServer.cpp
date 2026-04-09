//
// Created by harryqk on 4/5/26.
//

#include "TCPServer.h"
#include "SCPPSocketFactory.h"

#include "EventLoop.h"
#include "Channel.h"

#ifdef _WIN32


#elif __APPLE__

#include "./Poller/KqueuePoller.h"
#include "./Wakeup//PipeWakeup.h"
#include "./Mac/SCPPSocketFactoryMac.h"

#elif __ANDROID__

#elif __linux__
#include "./Poller/EpollPoller.h"
#include "./Wakeup//EventfdWakeup.h"
#include "./Linux/SCPPSocketFactoryLinux.h"
#endif
namespace SinaiEcho
{
    TCPServer::TCPServer(int Port)
    {

#ifdef _WIN32


#elif __APPLE__
        Factory = new SCPPSocketFactoryMac();
        KqueuePoller* poller = new KqueuePoller();
        PipeWakeup* wakeup = new PipeWakeup();

#elif __ANDROID__

#elif __linux__
        Factory = new SCPPSocketFactoryLinux();
        EpollPoller* poller = new EpollPoller();
        EventfdWakeup* wakeup = new EventfdWakeup();
#endif

        Local = Factory->CreateSocket(SocketAddressFamily::IPv4, SocketType::SOCKTYPE_Streaming, SocketProtocol::TCP);
        if(Local->GetFileDescriptor() == SOCKET_ERROR)
        {

            std::printf("CreateSocket Error\n");
            return;
        }
        //set NonBlockMode
        bool ret = Local->SetNonBlockMode(true);
        if(!ret)
        {

            std::printf("Set NonBlock mode fail");

            return;
        }
        //bind
        int result;
        result = Local->Bind(Port);
        if(result == SOCKET_ERROR)
        {
            std::printf("Bind fail");

            return;
        }
        //listen;
        result = Local->Listen(4);
        if(result == SOCKET_ERROR)
        {
            std::printf("Listen fail");
            return;
        }

        loop =  new EventLoop(poller, wakeup);
        listenfd_ = Local->GetFileDescriptor();
    }

    void TCPServer::Start()
    {
        if(!Local)
        {
            std::printf("start fail");
            return;
        }
        Channel* acceptChannel = new Channel(loop, listenfd_);
        loop->AddChannel(acceptChannel);
        acceptChannel->EnableReading();
        acceptChannel->SetReadCallback([&]() {
            HandleAccept();
        });


        std::cout << "server start at 8888...\n";

        loop->Loop();
        acceptChannel->DisableWriting();
        acceptChannel->DisableReading();
        delete acceptChannel;
    }

    void TCPServer::RemoveConnection(int fd)
    {
        Connections.erase(fd);
    }

    void TCPServer::Close()
    {

    }

    void TCPServer::HandleAccept()
    {
        if(!Local)
        {
            std::printf("HandleAccept fail");
        }
        int connfd = Local->Accept(listenfd_);
        Socket* Client = Factory->CreateConnectionSocket(connfd);
        //int connfd = accept(listenFd, nullptr, nullptr);

        if (connfd < 0)
        {
            // 非阻塞 accept 没连接了
            std::cout << "非阻塞 accept 没连接了" << connfd << std::endl;
        }
        else
        {
            std::cout << "new connection fd:=" << connfd << std::endl;

            //
            auto Connection = std::make_shared<NetConnection>(loop, std::unique_ptr<Socket>(Client));
            //NetConnection* Connection = new NetConnection(loop, std::unique_ptr<Socket>(Client));
            Connection->SetState(kConnected);
            Connection->GetChannel()->EnableReading();
            Connection->GetChannel()->EnableWriting();
            Connections.emplace(Client->GetFileDescriptor(), Connection);
            std::weak_ptr<NetConnection> weakConn = Connection;
            Connection->SetCloseCallback([this, weakConn](NetConnection*) {
                if (auto c = weakConn.lock())
                {
                    RemoveConnection(c->GetFd());
                    std::cout << "erase connection fd:=" << c->GetFd() << std::endl;
                }
            });

            Connection->SetMessageCallback([this, weakConn](const NetMessage& Msg) {
                if (auto c = weakConn.lock())
                {
                    if(OnMessage)
                    {
                        OnMessage(Msg);
                    }
                }
            });

        }
    }

    void TCPServer::SetMessageCallback(MessageCallback cb)
    {
        OnMessage = cb;
    }
}

