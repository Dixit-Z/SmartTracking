#ifndef CIBLE_H
#define CIBLE_H

#include <iostream>
#include <string>
#include "structs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/xfeatures2d.hpp"
#include "drone/AtCmd.hpp"

using namespace std;
using namespace cv;
using namespace cv::xfeatures2d;

class cible
{
	private :
		vector<KeyPoint> KP;
		Mat imgGray;
		Mat desc;
		int id;
		string name;
		bool foundTLD;
		bool foundCV;
		Rect rec;
		Mat picture;
		float densite;
		
		//Coordonnées de l'objet selon les différents tracking
		ObjCoord currentCVPos;
		ObjCoord currentTLDPos;
		ObjCoord currentRealPos;

		//Enregistrement des 15 dernières positions
		ListeObjCoord LastPos;

	public :
		float lastdZone;
		float roll;
		float pitch;
		float gaz;
		float yaw;
		cible(int id);
		void getName();
		void setName(string name);
		void setimgGray(Mat img);
		void setDesc(Mat img);
		void setKP(vector<KeyPoint> KP);
		void setPicture(Mat img);
		void setFoundCV(bool found);
		void setFoundTLD(bool found);
		void setCurrentTLD(float x, float y, float z);
		void setCurrentCV(float x, float y, float z);
		Mat getimgGray();
		Mat getDesc();
		vector<KeyPoint> getKP();
		Mat getPicture();
		bool getFoundCV();
		bool getFoundTLD();
		ObjCoord getRealPos();
		//Méthode permettant d'interprêter les résultats des deux tracking
		void setRealPos();
};

#endif
