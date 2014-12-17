#include <sstream>
#include <unistd.h>
#include "AtCmd.hpp"

UDP* AtCmd::udp;
int  AtCmd::seqNumber = 1;
string AtCmd::sessionId = "00000000";
string AtCmd::profileId = "00000000";
string AtCmd::applicationId = "00000000";

pthread_t* AtCmd::pidLoop = NULL;
pthread_mutex_t AtCmd::mutexLoop = PTHREAD_MUTEX_INITIALIZER;
useconds_t AtCmd::intervalLoop;


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

void AtCmd::initConfigIds(string sessionId, string profileId, string applicationId) {
    AtCmd::sessionId = sessionId;
    AtCmd::profileId = profileId;
    AtCmd::applicationId = applicationId;
    AtCmd::sendConfig("custom:session_id", AtCmd::sessionId);
    AtCmd::sendConfig("custom:profile_id", AtCmd::profileId);
    AtCmd::sendConfig("custom:application_id", AtCmd::applicationId);
}

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

void AtCmd::setLed(AtCmd::LedAnimation animation, float frequency, int duration) {
    AtCmd::send((string)"AT*LED=" + to_string(AtCmd::getNextSequence())
                                  + "," + to_string((int)animation)
                                  + "," + to_string(*(int*)(&frequency))
                                  + "," + to_string(duration));
}

void AtCmd::sendMovement(int flag, float roll, float pitch, float gaz, float yaw) {
    AtCmd::send((string)"AT*PCMD=" + to_string(AtCmd::getNextSequence())
                                   + "," + to_string(flag) + "," + to_string(*(int*)&roll)
                                   + "," + to_string(*(int*)&pitch) + "," + to_string(*(int*)&gaz)
                                   + "," + to_string(*(int*)&yaw));
}


void AtCmd::sendConfigIds() {
    AtCmd::send((string)"AT*CONFIG_IDS=" + to_string(AtCmd::getNextSequence())
                                         + ",\"" + AtCmd::sessionId + "\",\"" + AtCmd::profileId + "\",\"" + AtCmd::applicationId + "\"");
}

void AtCmd::sendConfig(string key, string value) {
    AtCmd::sendConfigIds();
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

void AtCmd::startLoop(int interval) {
    AtCmd::intervalLoop = (useconds_t) interval * 1000;
    pthread_mutex_lock(&AtCmd::mutexLoop);
    if(AtCmd::pidLoop == NULL) {
        AtCmd::pidLoop = (pthread_t*) malloc(sizeof(pthread_t));
        pthread_create(AtCmd::pidLoop, NULL, AtCmd::threadLoop, NULL);
    }
    pthread_mutex_unlock(&AtCmd::mutexLoop);
}

void AtCmd::stopLoop() {
    pthread_mutex_lock(&AtCmd::mutexLoop);
    if(AtCmd::pidLoop != NULL) {
        if (pthread_cancel(*AtCmd::pidLoop) == 0) {
            free(AtCmd::pidLoop);
            AtCmd::pidLoop = NULL;
        }
    }
    pthread_mutex_unlock(&AtCmd::mutexLoop);
}

void* AtCmd::threadLoop(void*){
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wmissing-noreturn"
    while(true){
        AtCmd::sendComWDG();
        usleep(AtCmd::intervalLoop);
        pthread_testcancel();
    }
    #pragma clang diagnostic pop
    return NULL;
}
