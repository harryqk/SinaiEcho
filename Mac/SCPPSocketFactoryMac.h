//
// Created by harryqk on 7/17/22.
//

#ifndef SINAIECHO_SCPPSOCKETFACTORYMAC_H
#define SINAIECHO_SCPPSOCKETFACTORYMAC_H
#include "../SCPPSocketFactory.h"
namespace SinaiEcho
{
    class SCPPSocketFactoryMac : public SCPPSocketFactory{
    public:
        SCPPSocketFactoryMac();
        virtual ~SCPPSocketFactoryMac() override;
        virtual Socket* CreateSocket(SocketAddressFamily AddressFamily, SocketType Type, SocketProtocol Protocol) override;
        virtual Socket * CreateConnectionSocket(int Fd) override;
        virtual sockaddr_in CreateAddress(const char* Address, int Port) override;
    };

}



#endif //SINAIECHO_SCPPSOCKETFACTORYMAC_H
