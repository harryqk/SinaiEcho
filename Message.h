//
// Created by harryqk on 7/19/22.
//

#ifndef SINAIECHO_MESSAGE_H
#define SINAIECHO_MESSAGE_H

namespace SinaiEcho
{
    class Message {
    public:
        Message(char* Data, int Len);
        ~Message();
        char* Data;
        int Len;
    };


}



#endif //SINAIECHO_MESSAGE_H
