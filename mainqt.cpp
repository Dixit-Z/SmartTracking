#include <cstdlib>
#include <iostream>
#include "utils/udp.hpp"
#include "drone/QAtCmd.hpp"
#include <unistd.h>
#include "video.hpp"
#include "gui/interface.hpp"

using namespace std;

//void testCallback(uint8_t* data, int length);
void* ack(void* arg);

int main(int argc, char ** argv) {

    /*cout << "Configuration du drone ..." << endl;
    QAtCmd::sendConfig("general:navdata_demo", "TRUE");
    QAtCmd::sendControl(QAtCmd::ControlMode::LogControl);
    QAtCmd::sendControl(QAtCmd::ControlMode::Idle);
    QAtCmd::sendControl(QAtCmd::ControlMode::LogControl);
    QAtCmd::sendConfig("video:video_channel", "0");
    sleep(1);*/

    /*QAtCmd::sendConfig("custom:session_id","d2e081a3");
    QAtCmd::sendConfig("custom:profile_id","be27e2e4");
    QAtCmd::sendConfig("custom:application_id","d87f7e0c");
    QAtCmd::sendConfig("control:control_vz_max","700");
    QAtCmd::sendConfig("control:control_yaw","1.727876");
    QAtCmd::sendConfig("control:euler_angle_max","0.209440");
    QAtCmd::sendConfig("control:altitude_max","3000");
    QAtCmd::sendConfig("video:bitrate_ctrl_mode","0");
    QAtCmd::sendConfig("video:bitrate","1000");
    QAtCmd::sendConfig("video:max_bitrate","4000");
    QAtCmd::sendConfig("video:video_codec","129");
    QAtCmd::sendConfig("video:video_channel","0");
    QAtCmd::sendConfig("video:video_on_usb","FALSE");
    QAtCmd::sendConfig("control:outdoor","FALSE");
    QAtCmd::sendConfig("control:flight_without_shell","FALSE");
    QAtCmd::sendConfig("general:navdata_demo","FALSE");*/

    /*cout << "Demarrage du flux video ..." << endl;
    pthread_t pid, ap;
    pthread_create(&pid, NULL, &camera, NULL);
    pthread_create(&ap , NULL, &ack, NULL);*/


    QApplication app(argc, argv);
    MyWidget myWidget;
    myWidget.show();
    
    return app.exec();
}

void* ack(void* arg) {
    while(1) {
        QAtCmd::sendComWDG();
        usleep(1000000);
    }
}

/*
void testCallback(uint8_t* data, int length){
    cout << "receive (" << length << ") : ";
    //printf("%.*s", length, data);
    cout << endl;
    udp.send(data, length);
}
*/
