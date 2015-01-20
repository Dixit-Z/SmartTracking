#include <cstdlib>
#include <iostream>
#include "utils/udp.hpp"
#include "drone/AtCmd.hpp"
#include "video.hpp"

using namespace std;

//void testCallback(uint8_t* data, int length);

int main(int argc, char ** argv) {

/*  QApplication app(argc, argv);
    QMainWindow main;
    Ui::MainWindow q;
    q.setupUi(&main);
    main.show();
    q.label->setText( "anc" );
    app.exec();
*/

    cout << "Configuration du drone ..." << endl;
    AtCmd::initConfigIds("d2e081a3", "be27e2e4", "d87f7e0c");
    AtCmd::sendConfig("control:control_vz_max","700");
    AtCmd::sendConfig("control:control_yaw","1.727876");
    AtCmd::sendConfig("control:euler_angle_max","0.209440");
    AtCmd::sendConfig("control:altitude_max","3000");
    AtCmd::sendConfig("control:autonomous_flight","FALSE");
    AtCmd::sendConfig("control:manual_trim","FALSE");
    AtCmd::sendConfig("control:outdoor","FALSE");
    AtCmd::sendConfig("control:flight_without_shell","FALSE");

    AtCmd::sendConfig("video:bitrate_control_mode","0");
    AtCmd::sendConfig("video:bitrate","1000");
    AtCmd::sendConfig("video:max_bitrate","4000");
    AtCmd::sendConfig("video:video_codec","129");
    AtCmd::sendConfig("video:video_channel","0");
    AtCmd::sendConfig("video:video_on_usb","FALSE");

    AtCmd::sendConfig("general:navdata_demo","FALSE");
    AtCmd::sendConfig("general:ardrone_name","ISTY AR DRONE");
    AtCmd::sendConfig("general:com_watchdog","2");
    AtCmd::sendConfig("general:video_enable","TRUE");
    AtCmd::sendConfig("general:vbat_min","9000");

    AtCmd::sendConfig("network:ssid_single_player","AR Drone ISTY 2014");

    AtCmd::sendControl(AtCmd::ControlMode::LogControl);

    cout << "Demarrage du flux video ..." << endl;
    pthread_t pid;
    pthread_create(&pid, NULL, &camera, NULL);
    AtCmd::startLoop(200);

    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wmissing-noreturn"
    while(1) {
        AtCmd::sendMovement(0, 0, 0, 0, 0);
        cout << "Attente de démarrage ..." << endl;
        AtCmd::setLed(AtCmd::LedAnimation::BLINK_RED, 2, 5);
        cin.get();
        AtCmd::sendFTrim();
        sleep(1);
        AtCmd::sendTakeOff();
        sleep(2);
        AtCmd::setLed(AtCmd::LedAnimation::BLINK_GREEN, 2, 5);

        cout << "Attente de tracking ..." << endl;
        cin.get();
        setActivate(1);
        AtCmd::setLed(AtCmd::LedAnimation::BLINK_ORANGE, 2, 5);

        cin.get();
        cout << "Atterissage ..." << endl;
        setActivate(0);
        AtCmd::sendLanding();
    }
    #pragma clang diagnostic pop

    cout << "Appuyez sur une touche pour quitter l'application." << endl;
    cin.get();

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
