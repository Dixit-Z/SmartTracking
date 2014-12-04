#include <sstream>
#include "AtCmd.hpp"

UDP* AtCmd::udp;
int  AtCmd::seqNumber = 1;

void AtCmd::send(string data) {
    // Ajout du retour a la ligne (CR)
    data.append("\x0D");

    // Creation du socket si celui-ci n'existe pas //
    if(AtCmd::udp == NULL) {
        AtCmd::udp = new UDP("192.168.1.1", 5556);
    }

    // Envoi du message //
    AtCmd::udp->send(data);
}

int AtCmd::getNextSequence() {
    return AtCmd::seqNumber++;
}

/////////////////////////// Public methods ///////////////////////////////////////////

void AtCmd::resetSequence() {
    AtCmd::seqNumber = 1;
}

void AtCmd::sendTakeOff() {
    AtCmd::send((string)"AT*REF=" + to_string(AtCmd::getNextSequence()) + ",290718208");
}

void AtCmd::sendLanding() {
    AtCmd::send((string)"AT*REF=" + to_string(AtCmd::getNextSequence()) + ",290717696");
}

void AtCmd::sendEmergency() {
    AtCmd::send((string)"AT*REF=" + to_string(AtCmd::getNextSequence()) + ",290717952");
}


void AtCmd::sendMovement(int flag, float roll, float pitch, float gaz, float yaw) {
    AtCmd::send((string)"AT*PCMD=" + to_string(AtCmd::getNextSequence())
                                   + "," + to_string(flag) + "," + to_string(*(int*)&roll)
                                   + "," + to_string(*(int*)&pitch) + "," + to_string(*(int*)&gaz)
                                   + "," + to_string(*(int*)&yaw));
}


void AtCmd::sendConfig(string key, string value) {
    AtCmd::send((string)"AT*CONFIG=" + to_string(AtCmd::getNextSequence())
                                     + ",\"" + key + "\",\"" + value + "\"");
}


void AtCmd::sendFTrim() {
    AtCmd::send((string)"AT*FTRIM=" + to_string(AtCmd::getNextSequence()));
}

void AtCmd::sendComWDG() {
    AtCmd::send((string)"AT*COMWDG=" + to_string(AtCmd::getNextSequence()));
}


void AtCmd::sendControl(ControlMode mode) {
    AtCmd::send((string)"AT*CTRL=" + to_string(AtCmd::getNextSequence())
                                   + "," + to_string((int)mode) + ",0");
}
