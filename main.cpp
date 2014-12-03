#include <cstdlib>
#include <iostream>
#include "utils/udp.hpp"
#include "drone/AtCmd.hpp"

using namespace std;

//void testCallback(uint8_t* data, int length);


int main() {
    AtCmd::sendConfig("general:navdata_demo", "TRUE");
    AtCmd::sendControl(AtCmd::ControlMode::LogControl);
    AtCmd::sendControl(AtCmd::ControlMode::Idle);
    AtCmd::sendControl(AtCmd::ControlMode::LogControl);
    AtCmd::sendConfig("video:video_channel", "0");
    AtCmd::sendFTrim();
    AtCmd::sendTakeOff();
    AtCmd::sendLanding();
    AtCmd::sendEmergency();
    AtCmd::sendComWDG();
    AtCmd::sendMovement(3, 1, 1, 1, 1);
    AtCmd::sendMovement(3, 0, 0, 0, 0);
    AtCmd::sendMovement(3, -1, -1, -1, -1);
    
//    udp.send("test");
  
    return 0;
}
/*
void testCallback(uint8_t* data, int length){
    cout << "receive (" << length << ") : ";
    //printf("%.*s", length, data);
    cout << endl;
    udp.send(data, length);
}
*/
