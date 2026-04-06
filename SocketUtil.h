//
// Created by harryqk on 7/22/22.
//

#ifndef SINAIECHO_SOCKETUTIL_H
#define SINAIECHO_SOCKETUTIL_H
#define byte unsigned char
namespace SinaiEcho
{
    class SocketUtil
    {
    public:
        static void IntToByte(int i,byte *bytes,int size = 4);
        static int BytesToInt(byte* bytes,int size = 4);
    };

}



#endif //SINAIECHO_SOCKETUTIL_H
