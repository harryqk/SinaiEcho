//
// Created by harryqk on 7/17/22.
//

#ifndef SINAIECHO_SCPPSOCKETFACTORY_H
#define SINAIECHO_SCPPSOCKETFACTORY_H
//#include "Socket.h"
#include "NetManagerWorker.h"
#include <list>
namespace SinaiEcho
{
    class SCPPSocketFactory {
    public:
        SCPPSocketFactory();
        virtual ~SCPPSocketFactory();
        virtual Socket* CreateSocket(SocketAddressFamily AddressFamily, SocketType Type, SocketProtocol Protocol) = 0;
        virtual Socket* CreateConnectionSocket(int Fd) = 0;
        virtual sockaddr_in CreateAddress(const char* Address, int Port) = 0;
    };
}




#endif //SINAIECHO_SCPPSOCKETFACTORY_H
