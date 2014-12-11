#include <cstdlib>
#include <iostream>
#include "utils/udp.hpp"
#include "drone/AtCmd.hpp"
#include <unistd.h>
#include "ui_main.h"
#include "video.hpp"

using namespace std;

//void testCallback(uint8_t* data, int length);


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
    AtCmd::sendConfig("general:navdata_demo", "TRUE");
    AtCmd::sendControl(AtCmd::ControlMode::LogControl);
    AtCmd::sendControl(AtCmd::ControlMode::Idle);
    AtCmd::sendControl(AtCmd::ControlMode::LogControl);
    AtCmd::sendConfig("video:video_channel", "0");
    sleep(1);

    cout << "Demarrage du flux video ..." << endl;
    pthread_t pid;
    pthread_create(&pid, NULL, &camera, NULL);

    cout << "Attente de démarrage ..." << endl;
    cin.get();
    AtCmd::sendFTrim();
    sleep(1);
    AtCmd::sendTakeOff();
    sleep(2);
    setActivate(1);

	cin.get();
    cout << "Atterissage ..." << endl;
    setActivate(0);
    AtCmd::sendLanding();

    cout << "Appuyez sur une touche pour quitter l'application." << endl;
    cin.get();

    AtCmd::sendMovement(3, 1, 1, 1, 1);
    AtCmd::sendMovement(3, 0, 0, 0, 0);
    AtCmd::sendMovement(3, -1, -1, -1, -1);

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
