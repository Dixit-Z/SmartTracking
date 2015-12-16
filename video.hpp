#ifndef VIDEO_H
#define VIDEO_H

#define ov_local 1
#define ov_remote_opencv 2
#define ov_remote_ffmpeg 4
#define output_video ov_remote_ffmpeg
//#define output_video ov_local

#include <iostream>
#include <unistd.h>
#include "drone/AtCmd.hpp"
#include <stdlib.h>
#include "structs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"
#include "cible.hpp"
#include "global.hpp"
// FFmpeg
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
}


using namespace cv;
using namespace cv::xfeatures2d;
using namespace std;

//int HH=179, HS=255, HV=255, LH=0, LS=0, LV=0;

//extern int HH, HS, HV, LH, LS, LV;

void drawCross(Mat f, int x, int y, Scalar color);
void* camera(void* arg);
void setActivate(int value);
void ajoutListe(ListeObjCoord *liste, ObjCoord newNoeud);
AVFormatContext * connexionWebcamDrone(AVFormatContext * Avformat);
void setAVFormatContext(AVFormatContext * AVFormat);
void *drawingAndParam(void * arg);
void MouseCallBack(int event, int x, int y, int flags, void* userdata);
void *matchTemplate(void * args);
void *opencv(void * args);
#endif
