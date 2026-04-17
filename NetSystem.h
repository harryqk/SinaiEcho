//
// Created by pc on 4/16/2026.
//

#ifndef SINAIECHO_NETSYSTEM_H
#define SINAIECHO_NETSYSTEM_H


namespace SinaiEcho
{
	class NetSystem {
    public:
        static bool Init();
        static void Shutdown();

    private:
        static bool s_inited;
	};

}



#endif //SINAIECHO_NETSYSTEM_H
