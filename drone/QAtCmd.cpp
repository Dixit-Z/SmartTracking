#include <sstream>
#include "QAtCmd.hpp"

UDP* QAtCmd::udp;
int  QAtCmd::seqNumber = 1;

void QAtCmd::send(string data) {
    // Ajout du retour a la ligne (CR)
    data.append("\x0D");

    // Creation du socket si celui-ci n'existe pas //
    if(QAtCmd::udp == NULL) {
        QAtCmd::udp = new UDP("192.168.1.1", 5556);
    }

    // Envoi du message //
    QAtCmd::udp->send(data);
}

int QAtCmd::getNextSequence() {
    return QAtCmd::seqNumber++;
}

/////////////////////////// Public methods ///////////////////////////////////////////

void QAtCmd::resetSequence() {
    QAtCmd::seqNumber = 1;
}

void QAtCmd::sendTakeOff() {
    QAtCmd::send((string)"AT*REF=" + to_string(QAtCmd::getNextSequence()) + ",290718208");
}

void QAtCmd::sendLanding() {
    QAtCmd::send((string)"AT*REF=" + to_string(QAtCmd::getNextSequence()) + ",290717696");
}

void QAtCmd::sendEmergency() {
    QAtCmd::send((string)"AT*REF=" + to_string(QAtCmd::getNextSequence()) + ",290717952");
}

void QAtCmd::sendMovement(int flag, float roll, float pitch, float gaz, float yaw) {
    QAtCmd::send((string)"AT*PCMD=" + to_string(QAtCmd::getNextSequence())
                                   + "," + to_string(flag) + "," + to_string(*(int*)&roll)
                                   + "," + to_string(*(int*)&pitch) + "," + to_string(*(int*)&gaz)
                                   + "," + to_string(*(int*)&yaw));
}


void QAtCmd::sendConfig(string key, string value) {
    QAtCmd::send((string)"AT*CONFIG=" + to_string(QAtCmd::getNextSequence())
                                     + ",\"" + key + "\",\"" + value + "\"");
}


void QAtCmd::sendFTrim() {
    QAtCmd::send((string)"AT*FTRIM=" + to_string(QAtCmd::getNextSequence()));
}

void QAtCmd::sendComWDG() {
    QAtCmd::send((string)"AT*COMWDG=" + to_string(QAtCmd::getNextSequence()));
}


void QAtCmd::sendControl(ControlMode mode) {
    QAtCmd::send((string)"AT*CTRL=" + to_string(QAtCmd::getNextSequence())
                                   + "," + to_string((int)mode) + ",0");
}

void QAtCmd::moveBW() {
    QAtCmd::sendMovement(3, 0, -1, 0, 0);
}

void QAtCmd::moveFW() {
    QAtCmd::sendMovement(3, 0, 1, 0, 0);
}

void QAtCmd::moveRight() {
    QAtCmd::sendMovement(3, 1, 0, 0, 0);
}

void QAtCmd::moveLeft() {
    QAtCmd::sendMovement(3, -1, 0, 0, 0);
}

void QAtCmd::goUp() {
    QAtCmd::sendMovement(3, 0, 0, 1, 0);
}

void QAtCmd::goDown() {
    QAtCmd::sendMovement(3, 0, 0,-1, 0);
}

void QAtCmd::yawRight() {
    QAtCmd::sendMovement(3, 0, 0, 0, 1);
}

void QAtCmd::yawLeft() {
    QAtCmd::sendMovement(3, 0, 0, 0, -1);
}
