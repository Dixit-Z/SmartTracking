#ifndef ATCMD_H
#define ATCMD_H

#include "../utils/udp.hpp"

using namespace std;

class AtCmd {
private:
    static UDP* udp;
    static int seqNumber;
    static void send(string data);
    static int getNextSequence();

    static string sessionId;
    static string profileId;
    static string applicationId;

public:
    static void initConfigIds(string sessionId, string profileId, string applicationId);
    static void resetSequence();

    static void sendTakeOff();
    static void sendLanding();
    static void sendEmergency();

    enum MovementFlag {
    };
    static void sendMovement(int flag, float roll, float pitch, float gaz, float yaw);

    static void sendConfig(string key, string value);
    static void sendConfigIds();

    static void sendFTrim();
    static void sendComWDG();

    enum class ControlMode {
        Idle = 0,
        SoftwareUpdateReception,
        PicSoftwareUpdateReception,
        Control,
        LogControl,
    };
    static void sendControl(ControlMode mode);

};
static AtCmd::MovementFlag operator+(AtCmd::MovementFlag const& a, AtCmd::MovementFlag const& b);

#endif
