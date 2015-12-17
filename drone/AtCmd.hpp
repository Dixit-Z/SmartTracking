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

    static pthread_t* pidLoop;
    static pthread_mutex_t mutexLoop;
    static useconds_t intervalLoop;
    static void* threadLoop(void*);

public:
    static void initConfigIds(string sessionId, string profileId, string applicationId);
    static void resetSequence();

    static void sendTakeOff();
    static void sendLanding();
    static void sendEmergency();

    enum class LedAnimation {
        BLINK_GREEN_RED              =  0,
        BLINK_GREEN                  =  1,
        BLINK_RED                    =  2,
        BLINK_ORANGE                 =  3,
        SNAKE_GREEN_RED              =  4,
        FIRE                         =  5,
        STANDARD                     =  6,
        RED                          =  7,
        GREEN                        =  8,
        RED_SNAKE                    =  9,
        BLANK                        = 10,
        RIGHT_MISSILE                = 11,
        LEFT_MISSILE                 = 12,
        DOUBLE_MISSILE               = 13,
        FRONT_LEFT_GREEN_OTHERS_RED  = 14,
        FRONT_RIGHT_GREEN_OTHERS_RED = 15,
        REAR_RIGHT_GREEN_OTHERS_RED  = 16,
        REAR_LEFT_GREEN_OTHERS_RED   = 17,
        LEFT_GREEN_RIGHT_RED         = 18,
        LEFT_RED_RIGHT_GREEN         = 19,
        BLINK_STANDARD               = 20,
        MAYDAY                       = 21
    };
    static void setLed(LedAnimation animation, float frequency, int duration);

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

    static void startLoop(int interval);
    static void stopLoop();

};
AtCmd::MovementFlag operator+(AtCmd::MovementFlag const& a, AtCmd::MovementFlag const& b);

#endif
