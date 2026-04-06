//
// Created by harryqk on 7/17/22.
//

#ifndef SINAIECHO_SCPPSOCKETFACTORYANDROID_H
#define SINAIECHO_SCPPSOCKETFACTORYANDROID_H
#include "../SCPPSocketFactory.h"
namespace SinaiEcho
{
    class SCPPSocketFactoryAndroid : public SCPPSocketFactory{
    public:
        SCPPSocketFactoryAndroid();
        virtual ~SCPPSocketFactoryAndroid() override;
        virtual Socket* CreateSocket(SocketAddressFamily AddressFamily, SocketType Type, SocketProtocol Protocol) override;
        virtual sockaddr_in CreateAddress(const char* Address, int Port) override;

    };

}



#endif //SINAIECHO_SCPPSOCKETFACTORYANDROID_H
