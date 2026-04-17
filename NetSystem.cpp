//
// Created by pc on 4/16/2026.
//

#include "NetSystem.h"
#include <iostream>
#ifdef _WIN32
#include <winsock2.h>
#endif
namespace SinaiEcho
{


    bool NetSystem::s_inited = false;

    bool NetSystem::Init()
    {
        if (s_inited) return true;

#ifdef _WIN32
        // Windows 需要初始化 Winsock


        WSADATA wsa;
        int ret = WSAStartup(MAKEWORD(2, 2), &wsa);
        if (ret != 0) {
            std::cerr << "WSAStartup failed: " << ret << std::endl;
            return false;
        }
#endif

        // Linux / macOS 什么都不用做
        s_inited = true;
        return true;
    }

    void NetSystem::Shutdown()
    {
        if (!s_inited) return;

#ifdef _WIN32
        WSACleanup();
#endif

        s_inited = false;
    }
}