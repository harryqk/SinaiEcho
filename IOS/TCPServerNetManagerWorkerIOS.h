//
// Created by harryqk on 7/21/22.
//

#ifndef SINAIECHO_TCPSERVERNETMANAGERWORKERIOS_H
#define SINAIECHO_TCPSERVERNETMANAGERWORKERIOS_H
#include "../NetManagerWorkerServer.h"
#include <list>
#include "../SocketUtil.h"
namespace scppsocket
{

    class TCPServerNetManagerWorkerIOS:public NetManagerWorkerServer
    {

    public:


    private:

        void HandleAccept();
        void HandleRead();
        fd_set readfds;
        fd_set testfds;

    public:
        TCPServerNetManagerWorkerIOS();
        virtual ~TCPServerNetManagerWorkerIOS() override;
        virtual void DoWork() override;
        virtual void SendMessage(int FileDescriptor, const char* Msg, int Len) override;
        virtual void Broadcast(const char *Msg, int Len) override;
        virtual void StopWork() override;
    };

}


#endif //SINAIECHO_TCPSERVERNETMANAGERWORKERIOS_H
