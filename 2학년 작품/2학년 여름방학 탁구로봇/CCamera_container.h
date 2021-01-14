#pragma once
#include <iostream>
#include <opencv2/opencv.hpp>
#include <thread>
#include <time.h>
#include <math.h>
#include "opencv2/videoio.hpp"
#include <opencv2/video.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include <omp.h>

using namespace cv;
using namespace std;

struct origin
{
	int x;
	int y;
};

class CCamera_container
{
public:
	CCamera_container(int camera_num);
	~CCamera_container();

	Mat* frame;
	Mat* imghsv;
	Mat* imgthresholded;
	Mat* imgLines;
	int iLastX, iLastY, posX, posY;
	int camera_num;
	origin* origin_point;
	int ilowh, ihighh, ilows, ihighs, ilowv, ihighv;
	VideoCapture cap;

	void polling();
};

