#include "CCamera_container.h"

void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
	CCamera_container* ptr = static_cast<CCamera_container*>(userdata);
	if (event == EVENT_LBUTTONDOWN)
	{
		ptr->origin_point->x = x;
		ptr->origin_point->y = y;

		std::cout << " -" + std::to_string(ptr->camera_num) << " camera's origin point : " << ptr->origin_point->x << ", " << ptr->origin_point->y << std::endl;
	}
}

CCamera_container::CCamera_container(int num)
{
	imghsv = new Mat();
	imgthresholded = new Mat();
	frame = new Mat();
	imgLines = new Mat();
	camera_num = num;
	iLastX = -1;
	iLastY = -1;
	posX = -1;
	posY = -1;

	cap = VideoCapture(num);
	cap.set(CV_CAP_PROP_FOURCC, CV_FOURCC('M', 'J', 'P', 'G'));

	cap.set(CAP_PROP_SETTINGS, 0);
	cap.set(CAP_PROP_FPS, 60);
	cap.set(CV_CAP_PROP_FRAME_WIDTH, 1280);  //1280 * 1024
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, 720);  // 640*480

	cap.read(*frame);
	*imgLines = Mat::zeros(frame->size(), CV_8UC3);

	cout << "\nDepth generator output mode:" << endl <<
		"FRAME_WIDTH      " << cap.get(CAP_PROP_FRAME_WIDTH) << endl <<
		"FRAME_HEIGHT     " << cap.get(CAP_PROP_FRAME_HEIGHT) << endl <<
		"FRAME_MAX_DEPTH  " << cap.get(CAP_PROP_OPENNI_FRAME_MAX_DEPTH) << " mm" << endl <<
		"FPS              " << cap.get(CAP_PROP_FPS) << endl <<
		"REGISTRATION     " << cap.get(CAP_PROP_OPENNI_REGISTRATION) << endl;


	origin_point = new origin;
	cv::namedWindow("original" + std::to_string(num), 1);
	cv::namedWindow("thresholded image" + std::to_string(num), 1);
	cv::setMouseCallback("original" + std::to_string(num), CallBackFunc, this);
}

void CCamera_container::polling()
{

	cap.read(*frame);

	cvtColor(*frame, *imghsv, COLOR_BGR2HSV);
	inRange(*imghsv, Scalar(ilowh, ilows, ilowv), Scalar(ihighh, ihighs, ihighv), *imgthresholded);

	erode(*imgthresholded, *imgthresholded, getStructuringElement(MORPH_RECT, Size(1, 1))); //침식연산 (낮은-어두운) : 작은노이즈 제거
	dilate(*imgthresholded, *imgthresholded, getStructuringElement(MORPH_RECT, Size(1, 1))); //팽창연산 (높은-밝은) : 큰객체로 결합

	dilate(*imgthresholded, *imgthresholded, getStructuringElement(MORPH_RECT, Size(1, 1)));
	erode(*imgthresholded, *imgthresholded, getStructuringElement(MORPH_RECT, Size(1, 1)));

	Moments oMoments = moments(*imgthresholded);

	double dM01 = oMoments.m01;
	double dM10 = oMoments.m10;
	double dArea = oMoments.m00;

	if (dArea > 500)
	{
		posX = dM10 / dArea;
		posY = dM01 / dArea;

		if (iLastX >= 0 && iLastY >= 0 && posX >= 0 && posY >= 0)
			line(*imgLines, Point(posX, posY), Point(iLastX, iLastY), Scalar(0, 0, 255), 2);

		iLastX = posX;
		iLastY = posY;
	}
	else
	{
		iLastX = -1;
		iLastY = -1;
		posX = -1;
		posY = -1;
	}

	(*frame) = (*frame) + (*imgLines);
}

CCamera_container::~CCamera_container()
{
}
