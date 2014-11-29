#include <iostream>
#include "AtCmd.hpp"
#include "../config.hpp"

UDP* AtCmd::udp;

void AtCmd::send(string data) {
    // Ajout du retour a la ligne (CR)
    data.append("\x0D");

    // Creation du socket si celui-ci n'existe pas //
    if(AtCmd::udp == NULL) {
        AtCmd::udp = new UDP("192.168.1.1", 5555);
    }

    // Envoi du message //
    AtCmd::udp->send(data);
}

void AtCmd::sendLanding() {
    AtCmd::send("testsss");
}

