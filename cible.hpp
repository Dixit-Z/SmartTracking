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
		//ADD TONIGHT
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
		ObjCoord currentCVPos;
		ObjCoord currentTLDPos;
		ObjCoord currentRealPos;
		//ListeObjCoord LastCVPos;
		//ListeObjCoord LastTLDPos;

	public :
		float lastdZone;
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
		void setRealPos();
};

#endif
