#include <cstdlib>
#include <iostream>
#include "utils/udp.hpp"
#include "drone/AtCmd.hpp"
#include <unistd.h>
#include "video.hpp"

using namespace std;

//void testCallback(uint8_t* data, int length);
void* ack(void* arg);

int main(int argc, char ** argv) {

/*    QApplication app(argc, argv);
    QMainWindow main;
    Ui::MainWindow q;
    q.setupUi(&main);
    main.show();
    q.label->setText( "anc" );
    app.exec();
*/

    cout << "Configuration du drone ..." << endl;
    //AtCmd::sendConfig("general:navdata_demo", "TRUE");
    /*AtCmd::sendControl(AtCmd::ControlMode::LogControl);
    AtCmd::sendControl(AtCmd::ControlMode::Idle);
    AtCmd::sendControl(AtCmd::ControlMode::LogControl);*/
    //AtCmd::sendConfig("video:video_channel", "0");
    //sleep(1);

    AtCmd::initConfigIds("d2e081a3", "be27e2e4", "d87f7e0c");
    AtCmd::sendConfig("control:control_vz_max","700");
    AtCmd::sendConfig("control:control_yaw","1.727876");
    AtCmd::sendConfig("control:euler_angle_max","0.209440");
    AtCmd::sendConfig("control:altitude_max","3000");
    AtCmd::sendConfig("video:bitrate_ctrl_mode","0");
    AtCmd::sendConfig("video:bitrate","1000");
    AtCmd::sendConfig("video:max_bitrate","4000");
    AtCmd::sendConfig("video:video_codec","129");
    AtCmd::sendConfig("video:video_channel","0");
    AtCmd::sendConfig("video:video_on_usb","FALSE");
    AtCmd::sendConfig("control:outdoor","FALSE");
    AtCmd::sendConfig("control:flight_without_shell","FALSE");
    AtCmd::sendConfig("general:navdata_demo","FALSE");

    cout << "Demarrage du flux video ..." << endl;
    pthread_t pid, ap;
    pthread_create(&pid, NULL, &camera, NULL);
    pthread_create(&ap , NULL, &ack, NULL);

    while(1) {
        cout << "Attente de démarrage ..." << endl;
        cin.get();
        AtCmd::sendFTrim();
        sleep(1);
        AtCmd::sendTakeOff();
        sleep(2);

        cout << "Attente de tracking ..." << endl;
        cin.get();
        setActivate(1);

        cin.get();
        cout << "Atterissage ..." << endl;
        setActivate(0);
        AtCmd::sendLanding();
    }

    cout << "Appuyez sur une touche pour quitter l'application." << endl;
    cin.get();

    return 0;
}

void* ack(void* arg) {
    while(1) {
        AtCmd::sendComWDG();
        usleep(500000);
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
