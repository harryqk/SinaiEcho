//
// Created by harryqk on 8/8/22.
//

#ifndef SINAIECHO_NETMANAGERWORKERCLIENT_H
#define SINAIECHO_NETMANAGERWORKERCLIENT_H
#include "NetManagerWorker.h"
namespace SinaiEcho
{
    class NetManagerWorkerClient:public NetManagerWorker
    {
    public:
        NetManagerWorkerClient();
        virtual ~NetManagerWorkerClient() override;
        void SetOnClientMessageReadDelegate(OnClientMessageReadDelegate Delegate);
        void SetOnConnectDelegate(OnConnectDelegate Delegate);

    protected:
        OnClientMessageReadDelegate OnClientMessageRead;

        OnConnectDelegate OnConnect;

    protected:

        char* LenBuf;
        char* ReadBuf;
    public:
        Connection *ConnectionToServer;
        Socket* Local;
        virtual void DoWork() override;
        virtual void SendMessage(const char *Msg, int Len) = 0;
        virtual void StopWork() override;
    };
}



#endif //SINAIECHO_NETMANAGERWORKERCLIENT_H
