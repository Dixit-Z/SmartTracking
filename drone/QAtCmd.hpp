#ifndef ATCMD_H
#define ATCMD_H

#include "../utils/udp.hpp"
#include <QObject>

using namespace std;

class QAtCmd: public QObject {
    Q_OBJECT
private:
    static int seqNumber;
    static void send(string data);
    static int getNextSequence();
    static UDP* udp;

public:

    static void resetSequence();
    enum MovementFlag {
    };
    
    enum class ControlMode {
        Idle = 0,
        SoftwareUpdateReception,
        PicSoftwareUpdateReception,
        Control,
        LogControl,
    };
    //static QAtCmd::MovementFlag operator+(QAtCmd::MovementFlag const& a, QAtCmd::MovementFlag const& b);
    static void sendConfig(string key, string value);
    static void sendControl(ControlMode mode);
    static void sendComWDG();
    static void sendMovement(int flag, float roll, float pitch, float gaz, float yaw);

public slots:
    static void sendTakeOff();
    static void sendLanding();
    static void sendEmergency();
    static void moveRight();
    static void moveLeft();
    static void goUp();
    static void goDown();
    static void yawRight();
    static void yawLeft();
    static void moveFW();
    static void moveBW();
    static void sendFTrim();
};

#endif
