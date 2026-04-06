//
// Created by harryqk on 7/17/22.
//

#ifndef SINAIECHO_SCPPSOCKETFACTORYWIN_H
#define SINAIECHO_SCPPSOCKETFACTORYWIN_H
#include "../SCPPSocketFactory.h"

namespace SinaiEcho
{
    class SCPPSocketFactoryWin : public SCPPSocketFactory{
    public:
        SCPPSocketFactoryWin();
        virtual ~SCPPSocketFactoryWin() override;
        virtual Socket* CreateSocket(SocketAddressFamily AddressFamily, SocketType Type, SocketProtocol Protocol) override;
        virtual sockaddr_in CreateAddress(const char* Address, int Port) override;
    };


}



#endif //SINAIECHO_SCPPSOCKETFACTORYWIN_H
