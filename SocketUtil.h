//
// Created by harryqk on 7/22/22.
//

#ifndef SINAIECHO_SOCKETUTIL_H
#define SINAIECHO_SOCKETUTIL_H

#pragma once
//#define byte unsigned char
namespace SinaiEcho
{

    class SocketUtil
    {
    public:
        static void IntToByte(int i, unsigned char* bytes, int size = 4);
        static int BytesToInt(unsigned char* bytes, int size = 4);
        static int GetLastError();
        static bool IsWouldBlock(int err);
        static bool IsInterrupted(int err);
        static bool IsInProgress(int err);
    };
}
#endif //SINAIECHO_SOCKETUTIL_H