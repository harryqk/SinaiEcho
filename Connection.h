//
// Created by harryqk on 7/17/22.
//

#ifndef SINAIECHO_CONNECTION_H
#define SINAIECHO_CONNECTION_H
#include "Socket.h"
#include "Message.h"
#include <queue>
#include <mutex>
namespace SinaiEcho
{
    class Connection {
    protected:
        Socket* SSock;
    public:
        Socket *GetSSock() const;

    public:
        explicit Connection(Socket* sSock);
        virtual ~Connection();

    protected:
        std::queue<Message*> SendMessage;
        std::queue<Message*> ReadMessage;
        std::mutex MutexSend;
        std::mutex MutexRead;
    public:
        virtual SockSSize_t Send(const char*buf, SockSize_t Len) = 0;
        virtual void PushSendMessage(char* buf, SockSize_t Len) = 0;
        virtual bool SendAll() = 0;
        virtual SockSSize_t Read(char* buf, SockSize_t Len) = 0;
        virtual void PushReadMessage(char* buf, SockSize_t Len) = 0;
        virtual Message* PopReadMessage() = 0;

    };

}


#endif //SINAIECHO_CONNECTION_H
