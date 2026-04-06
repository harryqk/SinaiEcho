//
// Created by harryqk on 7/17/22.
//

#ifndef SINAIECHO_SCPPSOCKETFACTORYLINUX_H
#define SINAIECHO_SCPPSOCKETFACTORYLINUX_H
#include "../SCPPSocketFactory.h"
namespace SinaiEcho
{
    class SCPPSocketFactoryLinux : public SCPPSocketFactory{
    public:
        SCPPSocketFactoryLinux();
        virtual ~SCPPSocketFactoryLinux() override;
        virtual Socket* CreateSocket(SocketAddressFamily AddressFamily, SocketType Type, SocketProtocol Protocol) override;
        virtual sockaddr_in CreateAddress(const char* Address, int Port) override;

    };

}



#endif //SINAIECHO_SCPPSOCKETFACTORYLINUX_H
