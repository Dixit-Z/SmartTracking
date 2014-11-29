#ifndef ATCMD_H
#define ATCMD_H

#include "../utils/udp.hpp"

using namespace std;

class AtCmd {
private:
    static UDP* udp;
    static void send(string data);

public:
    AtCmd();
    static void sendLanding();
    void sendTakeOff();
    void sendEmergency();
};

#endif