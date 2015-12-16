#include "video.hpp"
#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include "utils/udp.hpp"
#include "global.hpp"

using namespace std;
bool stopTracking=false;
bool enVol=false;
int main(int argc, char ** argv) {

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
	
	pthread_t id;
	pthread_create(&id, NULL, &drawingAndParam, NULL);
	AtCmd::startLoop(200);
	pthread_join(id,NULL);
	pthread_t pid;
	pthread_create(&pid, NULL, &camera, NULL);

	#pragma clang diagnostic push
	while(!stopTracking) {
		AtCmd::sendMovement(3,Ball.roll,Ball.pitch,Ball.gaz, Ball.yaw);
		if(enVol)
		{
			enVol=false;
			cout << "Attente de démarrage ..." << endl;
			AtCmd::setLed(AtCmd::LedAnimation::BLINK_RED, 2, 5);
			AtCmd::sendFTrim();
			//sleep(1);
			AtCmd::sendTakeOff();
			//sleep(2);
			AtCmd::setLed(AtCmd::LedAnimation::BLINK_GREEN, 2, 5);
			cout << "Attente de tracking ..." << endl;
			setActivate(1);
			AtCmd::setLed(AtCmd::LedAnimation::BLINK_ORANGE, 2, 5);
		}
		if(stopTracking)
		{
			cout << "Atterissage ..." << endl;
			setActivate(0);
			AtCmd::sendLanding();
		}
	}
	#pragma clang diagnostic pop

	return 0;
}
