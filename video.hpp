#ifndef VIDEO_H
#define VIDEO_H

#include <iostream>
#include <unistd.h>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

void drawCross(Mat f, int x, int y, Scalar color);
void* camera(void* arg);
void setActivate(int value);

#endif