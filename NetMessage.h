//
// Created by harryqk on 4/7/26.
//

#ifndef SINAIECHO_NETMESSAGE_H
#define SINAIECHO_NETMESSAGE_H
#pragma once
#include <functional>
#include <string>
namespace SinaiEcho
{
    struct NetMessage
    {
        int id;// 消息唯一标识，可用于日志追踪
        int fd;
        int type;           // 消息类型
        std::string Data;

    };
    using MessageCallback = std::function<void(const NetMessage&)>;
}



#endif //SINAIECHO_NETMESSAGE_H
