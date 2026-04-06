//
// Created by harryqk on 8/8/22.
//

#ifndef SINAIECHO_NETMANAGERWORKERSERVER_H
#define SINAIECHO_NETMANAGERWORKERSERVER_H
#include "NetManagerWorker.h"
#include <list>
namespace SinaiEcho
{
    class NetManagerWorkerServer:public NetManagerWorker
    {
    public:
        NetManagerWorkerServer();
        virtual ~NetManagerWorkerServer() override;
        void SetOnBindDelegate(OnBindDelegate Delegate);
        void SetOnAcceptDelegate(OnAcceptDelegate Delegate);
        void SetOnRemoveClientDelegate(OnRemoveClientDelegate Delegate);
        void SetOnServerMessageReadDelegate(OnServerMessageReadDelegate Delegate);
        virtual void SendMessage(int FileDescriptor, const char* Msg, int Len) = 0;
        virtual void Broadcast(const char *Msg, int Len) = 0;
    protected:
        OnBindDelegate OnBind;
        OnAcceptDelegate OnAccept;
        OnRemoveClientDelegate OnRemoveClient;
        OnServerMessageReadDelegate OnServerMessageRead;


        std::list<Connection*> ConnectionsToClient;
        Socket* Local;
    public:
        Socket *GetLocal() const;

        void SetLocal(Socket *local);

    protected:
        int MaxConnection;

        char* LenBuf;
        char* ReadBuf;
    public:
        int GetMaxConnection() const;
        virtual void DoWork() override;
        void SetMaxConnection(int maxConnection);
        virtual void StopWork() override;
    };
}



#endif //SINAIECHO_NETMANAGERWORKERSERVER_H
