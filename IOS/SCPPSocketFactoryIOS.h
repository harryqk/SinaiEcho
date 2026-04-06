//
// Created by harryqk on 7/17/22.
//

#ifndef SINAIECHO_SCPPSOCKETFACTORYIOS_H
#define SINAIECHO_SCPPSOCKETFACTORYIOS_H
#include "../SCPPSocketFactory.h"
namespace SinaiEcho
{
    class SCPPSocketFactoryIOS : public SCPPSocketFactory{
    public:
        SCPPSocketFactoryIOS();
        virtual ~SCPPSocketFactoryIOS() override;
        virtual Socket* CreateSocket(SocketAddressFamily AddressFamily, SocketType Type, SocketProtocol Protocol) override;
        virtual sockaddr_in CreateAddress(const char* Address, int Port) override;
    };

}



#endif //SINAIECHO_SCPPSOCKETFACTORYIOS_H
