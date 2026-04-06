//
// Created by harryqk on 7/17/22.
//

#include "Connection.h"
namespace SinaiEcho
{

    Socket *Connection::GetSSock() const
    {
        return SSock;
    }

    Connection::Connection(Socket* sSock)
    {
        SSock = sSock;
        std::printf("construct Connection\n");
    }

    Connection::~Connection()
    {
        std::printf("destruct Connection\n");
    }
}

