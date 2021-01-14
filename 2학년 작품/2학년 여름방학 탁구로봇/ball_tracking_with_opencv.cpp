#include "pch.h"

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include "include/GL/glew.h"		
#include "include/GLFW/glfw3.h" 

#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>

#include <math.h>
#include <chrono>
#include <utility>
#include <time.h>
#include <mutex>
#include <thread>
#include <algorithm>
#include <conio.h>

#include <WinSock2.h>
//#include <windows.h>
//#include <mmsystem.h>


//#include <omp.h>

#include <opencv2/opencv.hpp>

#include <librealsense2/rs.hpp>
#include <librealsense2/cv-helpers.hpp>
//#include "example.hpp""

//#include "CCamera_container.h"
#include "p3d_help.h"
#include "Transf_map.h"
#include "mat_help.h"
#include "Communication.h"
#include "event.h"

#include <librealsense2/hpp/rs_options.hpp>
#include <librealsense2/h/rs_option.h>




#define TEST cout << "** test **\n";

//<수정 금지>//
#define HUE8B_MAX		180
#define SATNVAL8B_MAX	255

#define HUE8B_MIN		0
#define SATNVAL8B_MIN	0

#define ENABLE			1
#define DISABLE			0

//---- control factor------//
#define BRIGHTNESS		1
#define GAIN			20
#define WHITE_BALANCE	300

#define BRIGHTNESS0		1
#define GAIN0			20
#define WHITE_BALANCE0	300

#define HUE_GRAVITY         7
#define SATURATION_GRAVITY  50
#define VALUE_GRAVITY       50

#define WIDTH 848
#define HEIGHT 480
#define FPS 60
// --------------------///


#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "lib/glew32.lib")
#pragma comment(lib, "lib/glfw3.lib")
//#pragma comment(lib, "winmm.lib")

#pragma comment(lib, "ws2_32")



using namespace cv;
using namespace rs2;
using namespace std;

Mat cam_img0,
	cam_img1,
  	rs_img;

void initialize(vector<pipeline> &pipelines, const int Brughtness = 30, const int gain = 50, const int White_balance = 30, 
	const int Brughtness0 = 30, const int gain0 = 50, const int White_balance0 = 30)
{
	pipeline pipe0, pipe1;
	config cfg0, cfg1;
	context ctx;
	cfg0.enable_stream(RS2_STREAM_COLOR, WIDTH, HEIGHT, RS2_FORMAT_BGR8, FPS);
	cfg1.enable_stream(RS2_STREAM_COLOR, WIDTH, HEIGHT, RS2_FORMAT_BGR8, FPS);

	auto dev0 = ctx.query_devices().front();
	cfg0.enable_device(dev0.get_info(RS2_CAMERA_INFO_SERIAL_NUMBER));
	auto dev1 = ctx.query_devices().back();
	cfg1.enable_device(dev1.get_info(RS2_CAMERA_INFO_SERIAL_NUMBER));

	vector<rs2::sensor> sensor1 = dev0.query_sensors();
	vector<rs2::sensor> sensor0 = dev1.query_sensors();


	sensor0.at(1).set_option(RS2_OPTION_BRIGHTNESS,				Brughtness0);
	sensor0.at(1).set_option(RS2_OPTION_GAIN,					gain0);
	sensor0.at(1).set_option(RS2_OPTION_WHITE_BALANCE,			2800 + White_balance0 * 10);
	sensor0.at(1).set_option(RS2_OPTION_ENABLE_AUTO_EXPOSURE,	ENABLE);

	sensor0.at(0).set_option(RS2_OPTION_LASER_POWER,			DISABLE);
	sensor0.at(0).set_option(RS2_OPTION_EMITTER_ENABLED,		DISABLE);

	sensor1.at(1).set_option(RS2_OPTION_BRIGHTNESS,				Brughtness);
	sensor1.at(1).set_option(RS2_OPTION_GAIN,					gain);
	sensor1.at(1).set_option(RS2_OPTION_WHITE_BALANCE,			2800 + White_balance * 10);
	sensor1.at(1).set_option(RS2_OPTION_ENABLE_AUTO_EXPOSURE,	ENABLE);

	sensor1.at(0).set_option(RS2_OPTION_LASER_POWER,			DISABLE);
	sensor1.at(0).set_option(RS2_OPTION_EMITTER_ENABLED,		DISABLE);


	pipe0.start(cfg0);
	pipe1.start(cfg1);
	pipelines.emplace_back(pipe1);
	pipelines.emplace_back(pipe0);
}


class Variable {

public:
	static int track1;
	static int track2;
	static int track3;
	static int track4;
	static int track5;
	static int track6;
	static int track7;
	static int track8;
	static int track9;
	static int track10;
	static int track11;
	static int track12;
	static int track13;
};
int Variable::track1 = HUE_GRAVITY;
int Variable::track2 = SATURATION_GRAVITY;
int Variable::track3 = VALUE_GRAVITY;
int Variable::track4 = 3;	// 수축
int Variable::track5 = 3;	// 팽창
int Variable::track6 = 0;	//rs_flag
int Variable::track7 = BRIGHTNESS;//30;	//BRIGHTNESS
int Variable::track8 = GAIN;//55;	//GAIN 
int Variable::track9 = WHITE_BALANCE;//300;	//WHITE_BALANCE
int Variable::track10 = BRIGHTNESS0;//30;	//BRIGHTNESS
int Variable::track11 = GAIN0;//55;	//GAIN 
int Variable::track12 = WHITE_BALANCE0;//300;	//WHITE_BALANCE
int Variable::track13 = 0;	//rs_flag


class Transf_Color_Area {
	int High1[3], Low1[3], High2[3], Low2[3], key = 0;
	int vaule[3];
	int * _track4 = &Variable::track4,
		*_track5 = &Variable::track5;
	Scalar hsv_Scalar[4];									//0:H1  1:L1  2:H2  3:L2
	unsigned int Transfer_Error[3] = { HUE_GRAVITY,
										SATURATION_GRAVITY,
										VALUE_GRAVITY };
	void set_color_Area(int Dimen) {							//Dimen : 0(h),1(s),2(v)    3(모두)

		if ((Dimen > 3) | (Dimen < 0)) {
			cout << "not correct Dimen !!" << endl;
			return;
		}
		if (Dimen == 3) {
			this->set_color_Area(0);
			this->set_color_Area(1);
			this->set_color_Area(2);

			return;
		}

		int high = vaule[Dimen] + Transfer_Error[Dimen];
		int low = vaule[Dimen] - Transfer_Error[Dimen];

		//Hue 설정
		if (Dimen == 0) {
			cout << setfill('0');

			if (low < HUE8B_MIN) {
				this->key = 1;

				High1[0] = HUE8B_MAX;
				Low1[0] = low + HUE8B_MAX;
				High2[0] = high;
				Low2[0] = HUE8B_MIN;
			}
			else if (high > HUE8B_MAX) {
				this->key = 2;

				High1[0] = HUE8B_MAX;
				Low1[0] = low;
				High2[0] = high - HUE8B_MAX;
				Low2[0] = HUE8B_MIN;
			}
			else {
				this->key = 3;

				High1[0] = high;
				Low1[0] = low;
				High2[0] = this->High1[0];
				Low2[0] = this->High1[0];
			}
			///범위가 1개 인경우를 else로 지정 범위가 1개임을 표시
			///위 또는 아래로 영역이 넘어간 경우(180또는 0) 범위2개를 이용해서 표기
		}
		//saturation, value 설정
		else {
			if (low < SATNVAL8B_MIN) {

				High1[Dimen] = high;
				Low1[Dimen] = SATNVAL8B_MIN;

				High2[Dimen] = High1[Dimen];
				Low2[Dimen] = Low1[Dimen];
			}
			else if (high > SATNVAL8B_MAX) {

				High1[Dimen] = SATNVAL8B_MAX;
				Low1[Dimen] = low;

				High2[Dimen] = High1[Dimen];
				Low2[Dimen] = Low1[Dimen];
			}
			else {

				High1[Dimen] = high;
				Low1[Dimen] = low;

				High2[Dimen] = this->High1[Dimen];
				Low2[Dimen] = this->Low1[Dimen];

			}
		}
	}//set_color_Area End

	void set_vaule_bgr(int b, int g, int r) {
		Scalar sca_hsv = bgr2hsv(b, g, r);

		this->vaule[0] = sca_hsv.val[0];
		this->vaule[1] = sca_hsv.val[1];
		this->vaule[2] = sca_hsv.val[2];
	}

	void set_hsv_Scalar() {
		hsv_Scalar[0] = Scalar(High1[0], High1[1], High1[2]);
		hsv_Scalar[1] = Scalar(Low1[0], Low1[1], Low1[2]);
		hsv_Scalar[2] = Scalar(High2[0], High2[1], High2[2]);
		hsv_Scalar[3] = Scalar(Low2[0], Low2[1], Low2[2]);
	}
public:

	Transf_Color_Area(const int& vaule1, const int& vaule2, const int& vaule3) {						//BGR색으로 초기화
		this->Set_color_bgr(vaule1, vaule2, vaule3);
	}
	Transf_Color_Area(Scalar s) {
		this->Set_color_bgr(s.val[0], s.val[1], s.val[2]);
	}
	void Set_color_bgr(const int& vaule1, const int& vaule2, const int& vaule3) {						//외부 호출용
		printf("                         (b,g,r) : (%.3d, %.3d, %.3d)    \n", vaule1, vaule2, vaule3);
		this->set_vaule_bgr(vaule1, vaule2, vaule3);
		this->set_color_Area(3);
		this->print_Area();
		this->set_hsv_Scalar();

		this->rgb_color_JPG();
	}

	void Set_Gravity(int vaule, int Dimen) {			//Dimen : 0(h),1(s),2(v)    3(모두)
		if ((Dimen > -1) || (Dimen < 3))
			Transfer_Error[Dimen] = vaule;
		else
			std::cout << endl << "error! piz recording!" << endl;
		this->set_color_Area(3);
		this->print_Area();
		this->set_hsv_Scalar();

		this->rgb_color_JPG();
	}

	void print_Area() const {

		std::cout
			<< "HSV Area Type <" << this->key << ">       (h,s,v) : (" << setw(3) << vaule[0] << ", " << setw(3) << vaule[1] << ", " << setw(3) << vaule[2] << ")" << endl
			<< "*******************< key : " << this->key << " >*******************" << endl;

		if (this->key == 3) {
			std::cout
				<< "                     hue : " << setw(3) << this->Low1[0] << "  ~  " << setw(3) << this->High1[0] << endl;
		}
		else if ((this->key == 1) | (this->key == 2)) {
			std::cout
				<< "                     hue : " << setw(3) << this->Low1[0] << " ~ 180(0) ~ " << setw(3) << this->High2[0] << endl;

		}
		else {
			std::cout << "\n\n\n     Key Error!!     \n\n\n";
			return;
		}

		std::cout
			<< "              saturation : " << setw(3) << this->Low1[1] << "  ~  " << setw(3) << this->High1[1] << endl
			<< "                   value : " << setw(3) << this->Low1[2] << "  ~  " << setw(3) << this->High1[2] << endl
			<< "*************************************************" << endl << endl << endl;

	}

	void rgb_color_JPG() const {

		Mat bgr_color_PNG = imread("color.jpg"),
			img_mask,
			gray_PNG;

		gray_PNG = this->binarization(bgr_color_PNG);

		cvtColor(gray_PNG + 70, img_mask, COLOR_GRAY2BGR);
		img_mask = ~(~img_mask + ~bgr_color_PNG);

		resize(img_mask, bgr_color_PNG, Size(302, 302), 0, 0, INTER_LINEAR);
		cv::imshow("color_test_bgr", bgr_color_PNG);
	}

	Scalar bgr2hsv(int b, int g, int r) const {
		Mat _bgr_color = Mat(1, 1, CV_8UC3, Scalar(b, g, r)),	//CV_8UC3 >> 8비트 1채널(180도 255 255)
			_hsv_color;

		cvtColor(_bgr_color, _hsv_color, COLOR_BGR2HSV);

		int h = _hsv_color.at<Vec3b>(0, 0)[0];
		int s = _hsv_color.at<Vec3b>(0, 0)[1];
		int v = _hsv_color.at<Vec3b>(0, 0)[2];

		return Scalar(h, s, v);
	}
	Scalar hsv2bgr(int h, int s, int v) const {
		Mat _hsv_color = Mat(1, 1, CV_8UC3, Scalar(h, s, v)),	//CV_8UC3 >> 8비트 1채널(180도 255 255)
			_bgr_color;

		cvtColor(_bgr_color, _hsv_color, COLOR_BGR2HSV);

		int b = _hsv_color.at<Vec3b>(0, 0)[0];
		int g = _hsv_color.at<Vec3b>(0, 0)[1];
		int r = _hsv_color.at<Vec3b>(0, 0)[2];

		return Scalar(b, g, r);
	}

	bool get_key() const {
		return key;
	}

	Scalar High_Scalar(int key = 0) const {
		return hsv_Scalar[0 + key * 2];
	}
	Scalar Low_Scalar(int key = 0) const {
		return hsv_Scalar[1 + key * 2];
	}

	Mat binarization(const Mat& _img) const {

		Mat _img_mask1,
			_img_mask2,
			_img_hsv;

		cvtColor(_img, _img_hsv, COLOR_BGR2HSV);

		inRange(_img_hsv,
			this->Low_Scalar(),
			this->High_Scalar(),
			_img_mask1);		//지정한 hsv 범위를 이용하여 영상을 이진화

		if ((this->key == 1) | (this->key == 2)) {
			inRange(_img_hsv,
				this->Low_Scalar(true),
				this->High_Scalar(true),
				_img_mask2);
			_img_mask1 |= _img_mask2;
		}

		//morphological closing 모폴로지 열림연산
		dilate(_img_mask1, _img_mask1, getStructuringElement(MORPH_ELLIPSE, Size(*_track5, *_track5)));
		erode(_img_mask1, _img_mask1, getStructuringElement(MORPH_ELLIPSE, Size(*_track4, *_track4)));

		return _img_mask1;
	}

	~Transf_Color_Area() {//소멸자
	}
};

struct TCA_list {

	Transf_Color_Area* _pa = nullptr,
					 * _pa0 = nullptr;
};

class On_track {
public:
	//hvs area
	static void track1(int track_vaule, void* userdata) {
			(((TCA_list*)userdata)->_pa0)->Set_Gravity(track_vaule, 0);
			(((TCA_list*)userdata)->_pa)->Set_Gravity(track_vaule, 0);
	}
	static void track2(int track_vaule, void* userdata) {
			(((TCA_list*)userdata)->_pa0)->Set_Gravity(track_vaule, 1);
			(((TCA_list*)userdata)->_pa)->Set_Gravity(track_vaule, 1);
	}
	static void track3(int track_vaule, void* userdata) {
			(((TCA_list*)userdata)->_pa0)->Set_Gravity(track_vaule, 2);
			(((TCA_list*)userdata)->_pa)->Set_Gravity(track_vaule, 2);
	}
	//noise
	static void track4(int track_vaule, void* userdata) {	}
	static void track5(int track_vaule, void* userdata) {	}
	//rs
	static void track6(int track_vaule, void* userdata) {
		vector<pipeline>* vp = (vector<pipeline>*)(userdata);
		vp->at(0).stop();
		vp->at(1).stop();
		vp->pop_back();
		vp->pop_back();
		initialize(*vp,
			Variable::track7,
			Variable::track8,
			Variable::track9, 
			Variable::track10,
			Variable::track11,
			Variable::track12
		);
	}
	static void track7(int track_vaule, void* userdata) {
		if (Variable::track6 = 0) {
			Variable::track10 = track_vaule;
		}
		else
			Variable::track7 = track_vaule;
	}
	static void track8(int track_vaule, void* userdata) {
		if (Variable::track6 = 0) {
			Variable::track11 = track_vaule;
		}
		else
			Variable::track8 = track_vaule;
	}
	static void track9(int track_vaule, void* userdata) {
		if (Variable::track6 = 0) {
			Variable::track12 = track_vaule;
		}
		else
			Variable::track9 = track_vaule;
	}
};

void img_Mouse_Event(int event, int x, int y, int flags, void* userdata)
{
	if (event == EVENT_LBUTTONDOWN)
	{
		uint r, g, b;
		if (x > 480) {
			x -= 480;
			r = cam_img0.at<Vec3b>(y, x)[2];
			g = cam_img0.at<Vec3b>(y, x)[1];
			b = cam_img0.at<Vec3b>(y, x)[0];
			((TCA_list*)userdata)->_pa0->Set_color_bgr(b, g, r);
		}
		else {
			r = cam_img1.at<Vec3b>(y, x)[2];
			g = cam_img1.at<Vec3b>(y, x)[1];
			b = cam_img1.at<Vec3b>(y, x)[0];
			((TCA_list*)userdata)->_pa->Set_color_bgr(b, g, r);
		}
		///포인터로 바꾸기
		// uchar __ = __.color.ptr<uchar>(x)
		// uchar __ = __.color.ptr<uchar>(y)
		///데이터 직접접근으로 바꾸기
		// uchar __ = __.data
	}
	else if (event == EVENT_RBUTTONDOWN) {
		std::cout << "            (x, y)        (" << x << ", " << y << ")" << endl;
	}
}

// 쓰레드 테스트를 위한 함수
//void move_actu_test(double pos)
//{
//	if (pos < 0)
//		pos += 25;
//	else
//		pos -= 25;
//	if (pos > max_position) {   // 리니어가 정해진 범위 안에 있을 때 수행 
//		pos = max_position;
//	}
//	else if (pos < -max_position) {
//		pos = -max_position;
//	}
//	if ((pos < max_position) & (pos > -max_position)) {   // 리니어가 정해진 범위 안에 있을 때 수행 
//		AxmMovePos(0, pos, vel, accel, dccel);  // 요것은 기존에 쓰던 함수 펄스출력이 종료가 되어야 함수탈출 
//	}
//	//AxmMovePos(0, 0, 50, 200, 200);
//}



int main() {
	//thread t(move_actu_test, cmd_position);  // 요거 쓰레드 테스트 해볼라고 넣은 것임
	//CLinear_actu C;
	//thread t(move_actu2);

	cv::namedWindow("Trackbar",				WINDOW_NORMAL);
	cv::namedWindow("color_test_bgr",		WINDOW_AUTOSIZE);
	cv::namedWindow("Binarization_img",		WINDOW_AUTOSIZE);
	cv::namedWindow("rs_img",				WINDOW_AUTOSIZE);

	resizeWindow("Trackbar", 300, 500);

	moveWindow("color_test_bgr",	500, 0);
	moveWindow("Trackbar",			1098, 330);
	moveWindow("Binarization_img",	630, 0);
	moveWindow("rs_img",			930, 0);


	int		posX = -1,
			posY = -1, // 중심점
			posX0 = -1,
			posY0 = -1; // 중심점
	double	Area = 0;

	double	alpha = 1.0;
	char	ch_buf1[5],
			ch_buf2[5],
			ch_buf3[5],
			ch_buf4[5];

	chrono::system_clock::time_point	tpStart,
		tpEnd;
	double								dTime = 0;
	int									cont_while = 0;

	Mat buf_img1,
		buf_img2,
		rs_img,
		img_hsv,
		img_mask,
		img_mask0,
		spectrum = Mat(848, 480, CV_8UC1, 1),
		spectrum0 = Mat(848, 480, CV_8UC1, 1);

	Transf_Color_Area tcArea(org);
	Transf_Color_Area tcArea0(org0);
	TCA_list TCA_list;							//설정한 색의 영역 범위에 따라서 색변환 횟수 지정
	TCA_list._pa = &tcArea;
	TCA_list._pa0 = &tcArea0;

	cell Cell;
	node_p3d Node_p3d;
	vect vec;

	vector<pipeline> pipelines;
	initialize(pipelines,
		Variable::track7,
		Variable::track8,
		Variable::track9,
		Variable::track10,
		Variable::track11,
		Variable::track12
	);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	cv::createTrackbar("hue :", "Trackbar", &Variable::track1, 90, On_track::track1,  &TCA_list);			//Trackbar event
	cv::createTrackbar("sat :", "Trackbar", &Variable::track2, 255, On_track::track2, &TCA_list);
	cv::createTrackbar("val :", "Trackbar", &Variable::track3, 255, On_track::track3, &TCA_list);
	cv::createTrackbar("수축 :", "Trackbar", &Variable::track4, 20, On_track::track4, nullptr);
	cv::createTrackbar("팽창 :", "Trackbar", &Variable::track5, 20, On_track::track5, nullptr);
	cv::createTrackbar("flag :",			"Trackbar", &Variable::track6, 001, On_track::track6, &pipelines);
	cv::createTrackbar("BRIGHTNESS :",		"Trackbar", &Variable::track7, 064, On_track::track7, nullptr);
	cv::createTrackbar("GAIN:",				"Trackbar", &Variable::track8, 064, On_track::track8, nullptr);
	cv::createTrackbar("WHITE_BALANCE :",	"Trackbar", &Variable::track9, 370, On_track::track9, nullptr);


	//윈도우에 콜백함수를 등록
	cv::setMouseCallback("rs_img",			img_Mouse_Event, &TCA_list);
	//cv::setMouseCallback("cam_img",		img_Mouse_Event, pa);
	//cv::setMouseCallback("depth_image",	img_Mouse_Event, pa);

	printf("sys : start while \n");
	int flag = 0;

	tpStart = chrono::system_clock::now();
	while (1)////////////////////////////////////////           <  START WHILE(1)  >           ////////////////////////////////////////////////////////////////
	{
#pragma omp parallel 
		{
#pragma omp sections
			{

#pragma omp section
				{
					auto data0 = pipelines.at(0).wait_for_frames();		// Wait for next set of frames from the camera	
					auto frame0 = data0.get_color_frame();

					cam_img0 = frame_to_mat(frame0);					// Convert RealSense frame to OpenCV matrix:
					flip(cam_img0, cam_img0, 1);
					transpose(cam_img0, cam_img0);

					img_mask0 = TCA_list._pa0->binarization(cam_img0);
					labels(cam_img0, img_mask0, spectrum0, posX0, posY0, Area);
				}
#pragma omp section
				{
					auto data1 = pipelines.at(1).wait_for_frames();
					auto frame1 = data1.get_color_frame();
					
					cam_img1 = frame_to_mat(frame1);
					flip(cam_img1, cam_img1, 1);
					transpose(cam_img1, cam_img1);

					line(cam_img1, Point(29, -10), Point(-24, 300), Scalar(0, 0, 0, 80), 50);
					line(cam_img1, Point(0, A2/2), Point(484, A2/2), Scalar(0, 0, 0, 80), A2);
					line(cam_img1, Point(454, -10), Point(503, 300), Scalar(0, 0, 0, 80), 50);
					line(cam_img1, Point(0, 829), Point(484, 829), Scalar(0, 0, 0, 80), 38);

					line(cam_img1, Point(4, 301), Point(4, 301), Scalar(0, 0, 0, 50), 30);
					line(cam_img1, Point(480, 301), Point(480, 301), Scalar(0, 0, 0, 50), 30);
					line(cam_img1, Point(430, A2+10), Point(450, A2+50), Scalar(0, 0, 0, 50), 30);
					img_mask = TCA_list._pa->binarization(cam_img1);
					labels(cam_img1, img_mask, spectrum, posX, posY, Area);
					
				}
			}
		}//End paralle
		if (vec.get_flag() == 1) {
			vec.Lmove0();
		}


		if ((posX == -1)&(posX0 == -1)) {
			if (vec.get_flag() != 1){
				vec.vect_clear();
			}
			_itoa_s(-99, ch_buf1, 5, 10);
			_itoa_s(-99, ch_buf2, 5, 10);
			_itoa_s(-99, ch_buf3, 5, 10);

			vec.Lmove0();
		}
		else if (posX == -1) {
			//PPP "왼 카메라 tracking error \n");
		}
		else if (posX0 == -1) {
			//PPP "오른카메라 tracking error \n");
		}
		else {
			Point3d p3d(posX, posY, 0);
			Point3d p3d0(posX0, posY0, 0);

			vec.push(p3d, p3d0);
			//Node_p3d.push(rp2p3d(p3d));

			_itoa_s((int)(p3d.x), ch_buf1, 5, 10);
			_itoa_s((int)(p3d.y), ch_buf2, 5, 10);
			_itoa_s((int)(p3d.z), ch_buf3, 5, 10);
		}
		drawing_auxiliary_line(cam_img1, cam_img0);
		hconcat(cam_img1, cam_img0, rs_img);
		drawing_auxiliary_line(rs_img, HEIGHT * 2, WIDTH);
		hconcat(spectrum, spectrum0, img_mask);

		//resize(rs_img, rs_img, Size(720, 636));
		resize(img_mask, img_mask, Size(320, 283));


		_itoa_s((int)dTime+0.1, ch_buf4, 5, 10);
		string	st_buf1(ch_buf1),
				st_buf2(ch_buf2),
				st_buf3(ch_buf3),
				st_buf4(ch_buf4);

		putText(rs_img, "x : " + st_buf1,	Point(5, 15), FONT_HERSHEY_PLAIN, 1.2, Scalar(255, 230, 100));
		putText(rs_img, "y : " + st_buf2,	Point(105, 15), FONT_HERSHEY_PLAIN, 1.2, Scalar(255, 230, 100));
		putText(rs_img, "z : " + st_buf3 + "     FPS : " + st_buf4, 
											Point(205, 15), FONT_HERSHEY_PLAIN, 1.2, Scalar(255, 230, 100));

		cv::imshow("Binarization_img", img_mask);//img_mask
		cv::imshow("rs_img", rs_img);

		if (int onkey = waitKeyEx(1)) {
			int cell_flag = 0;

			if (onkey == 8060928)
				udpClient(0.2, 20, 20);
			else if (onkey == 107);
			else
			{
				keybd_event(onkey, alpha, cell_flag);

				if (cell_flag == 1) {
					Cell.save_3d(Node_p3d);
					//Cell.save_graph(Node_p3d, m);
					//cv::imshow("m", m);
				}
				else if (cell_flag == 2) {
					Mat m;
					Cell.print_p3d(Node_p3d, m);\
					cv::imshow("m", m);
				}
			}
		}

		if (++cont_while == 100) {
			cont_while = 0;
			tpEnd = chrono::system_clock::now();
			dTime = 100000.0 / chrono::duration_cast<chrono::milliseconds>(tpEnd - tpStart).count();
			//cout << "FPS : " << dTime << endl;
			tpStart = tpEnd;
		}
	}//while End
}
