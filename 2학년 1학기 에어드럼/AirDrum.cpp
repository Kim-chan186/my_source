#include "pch.h"

#include <opencv2/opencv.hpp>
#include <iostream>

#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <mmsystem.h>
#include <string>
#include <algorithm>
#include <thread>

//<수정 금지>//
#define HUE_8B_MAX 180
#define SAT_OR_VAL_8B_MAX 255

#define HUE_8B_MIN 0
#define SAT_OR_VAL_8B_MIN 0


//<수정 가능>//
#define  HUE_GRAVITY         10
#define  SATURATION_GRAVITY  80
#define  VALUE_GRAVITY       90



//0.크래쉬,  1.하이헷,   2.스네어,   3.킥       4. 탐1
#define CRASH		PlaySound(TEXT(		"CYCdh_Kurz08-Crash01.wav"	), NULL, SND_ASYNC);
#define HHAT		PlaySound(TEXT(		"CYCdh_Sab_ClHat-04.wav"	), NULL, SND_ASYNC); //3-4
#define SNARE		PlaySound(TEXT(		"CYCdh_K2room_Snr-05.wav"		), NULL, SND_ASYNC);

//Acoustic Snare-04.wav
#define KIK			PlaySound(TEXT(		"CYCdh_AcouKick-20.wav"		), NULL, SND_ASYNC); //17 ~ 20
#define TAM1		PlaySound(TEXT(		"CYCdh_LudRimB-07.wav"		), NULL, SND_ASYNC);

#define TAM2		PlaySound(TEXT(		"CYCdh_PiccoloA-02.wav"		), NULL, SND_ASYNC);

//"CYCdh_PiccoloA-02.wav"
//"CYCdh_K5-Tom01c.wav"
#define OHAT		PlaySound(TEXT(		"CYCdh_K2room_OpHat-05.wav"		), NULL, SND_ASYNC);



//컴파일이 끝난 뒤 링크할떄 추가되는 파일
#pragma comment(lib, "winmm.lib")



using namespace cv;
using namespace std;


//차영상 difference image
//이전값과 변화를비교
//그레이 이미지를 사용함
//CUDA => GUI사용 => OpenCV다운 >> Cmake 설정(컴파일러 같은것, 경로설정 해줌)
//Cmake를 사용한 OpenCV > 쿠다    (8.0//9.0 추천)
//제어주기


class Transf_Color_Area {
	int High1[3], Low1[3], High2[3], Low2[3], key = 0;
	int vaule[3];
	unsigned int Transfer_Error[3] = { HUE_GRAVITY, SATURATION_GRAVITY, VALUE_GRAVITY };

public:
	Transf_Color_Area(const int& vaule1, const int& vaule2, const int& vaule3) {//생성자 > 기준 값 set 후, 색 영역지정
		this->set_Color(vaule1, vaule2, vaule3);
	}
	void set_Color(const int& vaule1, const int& vaule2, const int& vaule3) {//기준 값 set 후, 색 영역지정
		vaule[0] = vaule1;
		vaule[1] = vaule2;
		vaule[2] = vaule3;

		std::cout << endl << endl
			<< "----------< set Standard Color >-----------" << endl
			<< "       hue : " << vaule1 << endl
			<< "saturation : " << vaule2 << endl
			<< "     value : " << vaule1 << endl
			<< "------------------------------------------- "
			<< endl << endl;

		this->set_Color_Area();
	}

	void Set_Tolerance_Error(int vaule, int Dimen) {//가중치 변경 후, 색 영역 변경
		if ((Dimen > -1) || (Dimen < 3))
			Transfer_Error[Dimen] = vaule;
		else
			std::cout << endl << "error! piz recording!" << endl;


		this->set_Color_Area();//값은 1개만 변하는데 3번 계산함
	}

	void set_Color_Area() { //색 영역지정

		for (int i = 0; i < 3; i++) {
			int high = vaule[i] + Transfer_Error[i];
			int low = vaule[i] - Transfer_Error[i];

			//Hue 설정
			if (i == 0)
				if (low < HUE_8B_MIN) {
					std::cout << "Hue <1st> set" << endl;
					key = 1;

					High1[0] = HUE_8B_MAX;
					Low1[0] = low + HUE_8B_MAX;
					High2[0] = high;
					Low2[0] = HUE_8B_MIN;
				}
				else if (high > HUE_8B_MAX) {
					std::cout << "Hue <2nd> set" << endl;
					key = 1;

					High1[0] = HUE_8B_MAX;
					Low1[0] = low;
					High2[0] = high - HUE_8B_MAX;
					Low2[0] = HUE_8B_MIN;
				}
				else {
					std::cout << "Hue <3rd> set" << endl;
					key = 0;

					High1[0] = high;
					Low1[0] = low;
					High2[0] = this->High1[0];
					Low2[0] = this->High1[0];
				}///범위가 1개 인경우를 else로 지정 범위가 1개임을 표시
				///위 또는 아래로 영역이 넘어간 경우(180또는 0) 범위2개를 이용해서 표기

			//saturation, value 설정
			//영역 넘어가면 아래영역으로 채우는 코드로 수정하자
			else {
				if (low < SAT_OR_VAL_8B_MIN) {

					High1[i] = high;
					Low1[i] = SAT_OR_VAL_8B_MIN;

					High2[i] = High1[i];
					Low2[i] = Low1[i];
				}
				else if (high > SAT_OR_VAL_8B_MAX) {
					key = 1;

					High1[i] = SAT_OR_VAL_8B_MAX;
					Low1[i] = low;

					High2[i] = High1[i];
					Low2[i] = Low1[i];
				}
				else {

					High1[i] = high;
					Low1[i] = low;

					High2[i] = this->High1[i];
					Low2[i] = this->Low1[i];
				}
			}

		}
		this->print();

	}
	void print() {

		if (this->get_key() == 0) {
			std::cout << "hue = " << vaule[0] << "   //  saturation = " << vaule[1] << "   //  value = " << vaule[2] << endl;
			std::cout << "**************************< key : 0 >***************************" << endl;
			std::cout << "hue = " << this->High1[0] << "   //  saturation = " << this->High1[1] << "   //  value = " << this->High1[2] << endl;
			std::cout << "hue = " << this->Low1[0] << "   //  saturation = " << this->Low1[1] << "   //  value = " << this->Low1[2] << endl;
			std::cout << "***************************************************************" << endl << endl << endl;
		}
		else if (this->get_key() == 1) {
			std::cout << "hue = " << vaule[0] << "   //  saturation = " << vaule[1] << "   //  value = " << vaule[2] << endl;
			std::cout << "**************************< key : 1 >***************************" << endl;
			std::cout << "hue = " << this->High1[0] << "   //  saturation = " << this->High1[1] << "   //  value = " << this->High1[2] << endl;
			std::cout << "hue = " << this->Low1[0] << "   //  saturation = " << this->Low1[1] << "   //  value = " << this->Low1[2] << endl << endl;

			std::cout << "hue = " << this->High2[0] << "   //  saturation = " << this->High2[1] << "   //  value = " << this->High2[2] << endl;
			std::cout << "hue = " << this->Low2[0] << "   //  saturation = " << this->Low2[1] << "   //  value = " << this->Low2[2] << endl << endl;
			std::cout << "***************************************************************" << endl << endl << endl;
		}
		else {
			std::cout << "\n\n\n     Key Error!!     \n\n\n";
		}
	}

	int get_High(const int dimen, int key = 0) {//
		if (key == 0)
			return High1[dimen];
		else
			return High2[dimen];
	}
	int get_Low(const int dimen, int key = 0) {
		if (key == 0)
			return Low1[dimen];
		else
			return Low2[dimen];
	}

	bool get_key() {//
		return key;
	}


	Scalar High_Scalar(int key = 0) {//
		return Scalar(get_High(0, key), get_High(1, key), get_High(2, key));
	}
	Scalar Low_Scalar(int key = 0) {
		return Scalar(get_Low(0, key), get_Low(1, key), get_Low(2, key));
	}


	~Transf_Color_Area() {//소멸자

	}

};

Mat cpm_img;

class Variable {
public:
	static int track1;
	static int track2;
	static int track3;
	static int track4;
	static int track5;
	//static Mat cpm_img;

};
//Mat Variable::cpm_img(1, 1, CV_8UC3, Scalar(0, 0, 0));
int Variable::track1 = HUE_GRAVITY;
int Variable::track2 = SATURATION_GRAVITY;
int Variable::track3 = VALUE_GRAVITY;
int Variable::track4 = 3; // 수축
int Variable::track5 = 3; // 팽창


class On_track {
public:
	static void track1(int track_vaule, void* userdata) {
		((Transf_Color_Area*)userdata)->Set_Tolerance_Error(track_vaule, 0);
	}
	static void track2(int track_vaule, void* userdata) {
		((Transf_Color_Area*)userdata)->Set_Tolerance_Error(track_vaule, 1);
	}
	static void track3(int track_vaule, void* userdata) {
		((Transf_Color_Area*)userdata)->Set_Tolerance_Error(track_vaule, 2);
	}
	static void track4(int track_vaule, void* userdata) {
		if (track_vaule == 0) {
			track_vaule = 1;
			return;
		}
		track_vaule = (*(int*)userdata);
	}
	static void track5(int track_vaule, void* userdata) {
		if (track_vaule == 0){
			track_vaule = 1;
			return;
		}
		track_vaule = (*(int*)userdata);
	}
};

void Mouse_Event(int event, int x, int y, int flags, void* userdata)
{

	if (event == EVENT_LBUTTONDOWN)
	{
		uint r = cpm_img.at<Vec3b>(y, x)[2];
		uint g = cpm_img.at<Vec3b>(y, x)[1];
		uint b = cpm_img.at<Vec3b>(y, x)[0];
		///포인터로 바꾸기
		// uchar __ = __.color.ptr<uchar>(x)
		// uchar __ = __.color.ptr<uchar>(y)
		///데이터 직접접근으로 바꾸기
		// uchar __ = __.data

		std::cout << "     ┌ < Left Mouse Event >┐     " << endl;
		std::cout << "     │     " << r << ", " << g << ", " << b << endl;
		std::cout << "     └────────         " << endl;

		Scalar bgr(b, g, r);
		Scalar hsv;

		Mat mou_color(250, 250, CV_8UC3, Scalar(b, g, r));
		Mat hsv_color;
		cvtColor(mou_color, hsv_color, COLOR_BGR2HSV);

		int h = hsv_color.at<Vec3b>(0, 0)[0];
		int s = hsv_color.at<Vec3b>(0, 0)[1];
		int v = hsv_color.at<Vec3b>(0, 0)[2];
		((Transf_Color_Area*)userdata)->set_Color(h, s, v);

		cv::imshow("컬러 테스트", mou_color);
		cv::imshow("hsv테스트 색", hsv_color);
		
	}
	if (event == EVENT_RBUTTONDOWN) {
		std::cout << "오른쪽 마우스 버튼 클릭.. 좌표 = (" << x << ", " << y << ")" << endl;
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//https://kgh1994.tistory.com/4?category=694173



//연산자 오버로딩
bool operator < (const Point& p1, const Point& p2) {
	return (p1.x < p2.x) & (p1.y < p2.y);
}
double operator * (const Point& p1, const Point& p2) {
	return pow((p1.x -p2.x),2) + pow((p1.y - p2.y), 2);
}
Point operator + (Point& p1, const int& n) {
	Point p = p1;
	p.x += n;
	p.y += n;
	return p;
}
Point operator - (Point& p1, const int& n) {
	Point p = p1;
	p.x -= n;
	p.y -= n;
	return p;
}

void func_Crash(){
	CRASH
}

void func_Hhat() {
	HHAT
}
void func_Snare() {
	SNARE
}
void func_Tam1() {
	TAM1
}
void func_Tam2() {
	TAM2
}
void func_OHat(){
	OHAT
}



//소리 파일 있는 곳
//https://selky.iptime.org/Selky/?p=4834
class Drum {

	       //0.크래쉬,  1.하이헷,   2.스네어,   3.킥       4. 탐1
	Point _ptCrash[2], _ptHhat[2], _ptSnare[2],            _ptTam1[2], _ptTam2[2], _ptOHat[2];
	bool flag[7];
	thread thr_Crash, thr_Hhat, thr_Snare, thr_Tam1, thr_Tam2, thr_OHat;
	
	auto In_xy(const Point*_pt, const Point& p) -> bool { return ((*_pt < p) & (p < *(_pt + 1))); };

public :
	Drum(const Point *Crash, const Point *Hhat, const Point *Snare, const Point *Tam1, const Point *Tam2, const Point *OHat) {

		//Point set
		_ptCrash[0] = *Crash;
		_ptCrash[1] = *(Crash+1);

		_ptHhat[0] = *Hhat;
		_ptHhat[1] = *(Hhat + 1);

		_ptSnare[0] = *Snare;
		_ptSnare[1] = *(Snare + 1);

		_ptTam1[0] = *Tam1;
		_ptTam1[1] = *(Tam1 + 1);

		_ptTam2[0] = *Tam2;
		_ptTam2[1] = *(Tam2 + 1);

		_ptOHat[0] = *OHat;
		_ptOHat[1] = *(OHat + 1);

		

		//flag 초기화
		for (int i = 0; i < 7; i++) {
			flag[i] = 0;
		}

	};

	bool& get_flag() {
		return flag[0];
	}

	//소리출력, 박스 그리기
	void Make_Durm(const Point point) {
		
		//0. 크래쉬
		if ( In_xy(_ptCrash,point) ) {
			rectangle(cpm_img, _ptCrash[0], _ptCrash[1], Scalar(0, 0, 255), 2);

			if (flag[0] == 0) {
				flag[0] = 1;
				//Crash;
				CRASH;
			}
		}
		else {
			
			flag[0] = 0;
		}
		
		//1. 하이헷
		if (In_xy(_ptHhat, point) ) {

			rectangle(cpm_img, _ptHhat[0], _ptHhat[1], Scalar(0, 0, 255), 2);

			if (flag[1] == 0) {
				flag[1] = 1;
				//Hhat;
				HHAT;
			}
		}
		else {
			
			flag[1] = 0;
		}

		//2. 스네어
		if ( In_xy(_ptSnare, point) ) {

			rectangle(cpm_img, _ptSnare[0], _ptSnare[1], Scalar(0, 0, 255), 2);

			if (flag[2] == 0) {
				flag[2] = 1;
				//Snare;
				SNARE;
			}
		}
		else {
			
			flag[2] = 0;
		}
		//4. 탐1
		if (In_xy(_ptTam1, point)) {

			rectangle(cpm_img, _ptTam1[0], _ptTam1[1], Scalar(0, 0, 255), 2);

			if (flag[4] == 0) {
				flag[4] = 1;
				//Tam1;
				TAM1;
			}
		}
		else {
			
			flag[4] = 0;
		}
		//5. 탐2
		if (In_xy(_ptTam2, point)) {

			rectangle(cpm_img, _ptTam2[0], _ptTam2[1], Scalar(0, 0, 255), 2);

			if (flag[5] == 0) {
				flag[5] = 1;
				//Tam2;
				TAM2;
			}
		}
		else {

			flag[5] = 0;
		}

		//6. 오픈 헷
		if (In_xy(_ptOHat, point)) {

			rectangle(cpm_img, _ptOHat[0], _ptOHat[1], Scalar(0, 0, 255), 2);

			if (flag[6] == 0) {
				flag[6] = 1;
				//OHat;
				OHAT;
			}
		}
		else {

			flag[6] = 0;
		}
		
	}

	//사각형 그리
	void Ract() {
		rectangle(cpm_img, _ptSnare[0], _ptSnare[1], Scalar(0, 255, 0), 2);
		rectangle(cpm_img, _ptCrash[0], _ptCrash[1], Scalar(0, 255, 0), 2);
		rectangle(cpm_img, _ptHhat[0], _ptHhat[1], Scalar(0, 255, 0), 2);
		rectangle(cpm_img, _ptOHat[0], _ptOHat[1], Scalar(0, 255, 0), 2);
		rectangle(cpm_img, _ptTam1[0], _ptTam1[1], Scalar(0, 255, 0), 2);
		rectangle(cpm_img, _ptTam2[0], _ptTam2[1], Scalar(0, 255, 0), 2);
	}


};



int main() {

	//float time0 = 0.f;
	//LARGE_INTEGER cpuStart0, cpuStop0, cpuFreq;

	//QueryPerformanceFrequency(&cpuFreq); // 컴퓨터의 주파수 계산
	//QueryPerformanceCounter(&cpuStart0); // 시작 카운터 저장


////////////////////////////////////////////

	int range_count = 0;			//일반 변수

	Scalar red(0, 0, 255);			//색상 지정
	Scalar blue(255, 0, 0);
	Scalar green(0, 255, 0);

	Mat bgr_color = Mat(250, 250, CV_8UC3, red);
	//이미지 객체 생성  1*1픽셀크기
	//CV_8UC3 >> 8비트 1채널(180도 255 255)
	//(0, 0, 255)로 초기화
	cv::imshow("컬러 테스트", bgr_color);
	Mat hsv_color;


	cvtColor(bgr_color, hsv_color, COLOR_BGR2HSV);
	//값을 계산하면서 저장하는 방식
	//변환 전후 객체가 달라야 정상적으로 작동


	int hue = (int)hsv_color.at<Vec3b>(0, 0)[0];       //값들을 변수에 저장
	///포인터로 변환 안되나?
	//되더라
	int saturation = (int)hsv_color.at<Vec3b>(0, 0)[1];
	int value = (int)hsv_color.at<Vec3b>(0, 0)[2];


	Transf_Color_Area Area(hue, saturation, value);
	//설정한 색을 기준으로 허용 영역을 만듦
	Transf_Color_Area* pa = &Area;
	//설정한 색의 영역 범위에 따라서 색변환 횟수 지정



	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	VideoCapture cap(2);    //0번 카메라(노트북 웹캠)을 사용
							//현시간에 대한 영상을 cpa객체로부터 가져올 수 있음
	//Mat img_frame;		 //클래스 사용안되서 전역으로 처리함

	Mat img_hsv;			//이미지를 저장할 객체 2개 선언

	if (!cap.isOpened()) {
		cerr << "ERROR! Unable to open camera\n";
		return -1;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	Point Crash[2] = { Point(50, 100), Point(125, 225) };
	Point Hhat[2] = { Point(150, 330), Point(275, 425) }; 
	Point Ohat[2] = { Point(40, 300), Point(130, 400) };
	Point Snare[2] = { Point(300, 350), Point(420, 465) };
	Point Tam1[2] = { Point(450, 315), Point(540, 410) };
	Point Tam2[2] = { Point(560, 275), Point(700, 400) };

	Drum drum1(	Crash,		 //  0. 크레쉬
				Hhat,		 //  1. 하이헷
				Snare,		 //  2. 스네어
							 //  3. 킥
				Tam1,		 //  4. 탐1
				Tam2,		 //  5. 탐1
				Ohat		 //  6. 탐1
			);

	Drum drum2(Crash,		 //  0. 크레쉬
		Hhat,				 //  1. 하이헷
		Snare,				 //  2. 스네어
							 //  3. 킥
		Tam1,				 //  4. 탐1
		Tam2,				 //  5. 탐1
		Ohat				 //  6. 탐1
	);
	/*
	bool* pb = &drum1.get_flag();

	thread t([&pb]() {
		for (;;) {
			if (*pb == 1) {
				cout << "*pb : " << *pb <<endl;
				*pb == 2;
				CRASH;
			}

		}
	});*/





	Drum* pDrum[2] = { &drum1, &drum2};

	Mat img_mask1;
	Mat img_mask2;


	Point point_past[2] = { Point(0,0),Point(420,0) };
	Point point[2];


	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	
	
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	

	bool kik_flag = 1;
	int count = 0;			// 반복 횟수

	for (;;) {
		count++;
		//cout << count << endl;
		/*cout << "-----< start >-----" << endl;
		QueryPerformanceCounter(&cpuStop0);
		cout << "프로세스 시간 = " << (float)(cpuStop0.QuadPart - cpuStart0.QuadPart)/ (float)cpuFreq.QuadPart << "s" << endl;
		QueryPerformanceCounter(&cpuStart0);
		*/
		// wait for a new frame from camera and store it into 'frame'
		cap.read(img_mask1);
		flip(img_mask1, cpm_img, 1);
		// check if we succeeded
		if (cpm_img.empty()) {
			cerr << "ERROR! blank frame grabbed\n";
			return 0;
		}


		//hsv로 변환
		cvtColor(cpm_img, img_hsv, COLOR_BGR2HSV);

		//cv::imshow("hsv변환영상", img_hsv);

		//지정한 hsv 범위를 이용하여 영상을 이진화

		inRange(img_hsv, pa->Low_Scalar(), pa->High_Scalar(), img_mask1);
		if (pa->get_key()) {
			inRange(img_hsv, pa->Low_Scalar(true), pa->High_Scalar(true), img_mask2);
			img_mask1 |= img_mask2;
		}///이진화를 할 색상이 영역이 2개의 부분으로 지정된 경우(빨간색) 2번째 영역에대한 이진화를 해 더함
		//검정부분의 값이 0임으로 비트연산으로 더하면흰 부분의 색상이 겹침




		//morphological opening 작은 점들을 제거(수축)
		erode(img_mask1, img_mask1, getStructuringElement(MORPH_ELLIPSE,
			Size(Variable::track4, Variable::track4)));

		dilate(img_mask1, img_mask1, getStructuringElement(MORPH_ELLIPSE,
			Size(Variable::track4 + Variable::track5, Variable::track4)));


		//morphological closing 영역의 구멍 메우기(팽창)
		dilate(img_mask1, img_mask1, getStructuringElement(MORPH_ELLIPSE,
			Size(Variable::track5, Variable::track5)));

		erode(img_mask1, img_mask1, getStructuringElement(MORPH_ELLIPSE,
			Size(Variable::track5, Variable::track5)));


		if (_kbhit()) {

			if (GetKeyState(0x41) & 0xff0) {
			}
			else if (GetKeyState(0x41) & 0xff0) {
				if ((kik_flag == 1)) {
					kik_flag = 0;
					KIK;
				}
			}
			else {
				kik_flag = 1;
			}

		}

		//라벨링 
		Mat img_labels, stats, centroids;
		int numoflables = connectedComponentsWithStats(img_mask1, img_labels,
			stats, centroids, 8, CV_32S);



		//라벨링
		int max[2] = { -1 - 1 }, idx[2] = { 0, 0 };

		for (int j = 1; j < numoflables; j++) {
			int area = stats.at<int>(j, CC_STAT_AREA);

			if (area > 100) {

				if (*max < area) {
					*(max + 1) = *max;
					*(idx + 1) = *idx;

					*max = area;
					*idx = j;
				}
				else if (*(max + 1) < area) {
					*(max + 1) = area;
					*(idx + 1) = j;
				}
			}
		}


		//유효 라벨 찾기
		for (int j = 1; j < numoflables; j++) {

			//모든 라벨 그리기
			int left = stats.at<int>(j, CC_STAT_LEFT);
			int top = stats.at<int>(j, CC_STAT_TOP);
			int width = stats.at<int>(j, CC_STAT_WIDTH);
			int height = stats.at<int>(j, CC_STAT_HEIGHT);


			if ((j == *idx) | (j == *(idx + 1))) {
				//사용할 라벨 그리기
				rectangle(cpm_img, Point(left, top), Point(left + width, top + height),
					Scalar(255, 0, 255), 2);

			}
			else {
				//사용하지 않는 라벨 그리기
				rectangle(cpm_img, Point(left, top), Point(left + width, top + height),
					Scalar(0, 0, 170), 1);
			}

		}

		pDrum[0]->Ract();

		for (int i = 0; (i < numoflables)&(i < 2); i++) {
			int left = stats.at<int>(*(idx + i), CC_STAT_LEFT);
			int top = stats.at<int>(*(idx + i), CC_STAT_TOP);
			int width = stats.at<int>(*(idx + i), CC_STAT_WIDTH);
			int height = stats.at<int>(*(idx + i), CC_STAT_HEIGHT);


			//int y = top + height;
			//int x = left + width / 2;
			point[i] = Point(left + width / 2, top + height);
		}

		if ((point[0] * point_past[0]) >
			(point[0] * point_past[1])) {

			Point point_buf = point[0];
			point[0] = point[1];
			point[1] = point_buf;
		}

		//중간점
		rectangle(cpm_img, point[0], point[0] + 10,
			Scalar(255, 255, 0), 2);
		rectangle(cpm_img, point[1], point[1] + 10,
			Scalar(255, 0, 255), 2);

		//소리 출력
		pDrum[0]->Make_Durm(*(point + 0));
		pDrum[1]->Make_Durm(*(point + 1));


		//위치 값 저장
		*(point_past + 0) = *(point + 0);
		*(point_past + 1) = *(point + 1);


	
		//윈도우 생성
		cv::namedWindow("이진화 영상", WINDOW_AUTOSIZE);
		cv::namedWindow("원본 영상", WINDOW_AUTOSIZE);
		cv::namedWindow("트렉바", WINDOW_AUTOSIZE);


		//윈도우 출력
		cv::imshow("이진화 영상", img_mask1);
		cv::imshow("원본 영상", cpm_img);


		//"트렉바"윈도우에 트렉바 생성, 이벤트 등록
		cv::createTrackbar("hue :", "트렉바", &Variable::track1, 90, On_track::track1, pa);
		cv::createTrackbar("sat :", "트렉바", &Variable::track2, 255, On_track::track2, pa);
		cv::createTrackbar("val :", "트렉바", &Variable::track3, 255, On_track::track3, pa);
		cv::createTrackbar("수축 :", "트렉바", &Variable::track4, 20, On_track::track4, pa);
		cv::createTrackbar("팽창 :", "트렉바", &Variable::track5, 20, On_track::track5, pa);
		//0일때 죽는거 수정 해야함


		//윈도우에 콜백함수를 등록
		cv::setMouseCallback("원본 영상", Mouse_Event, pa);



		cv::waitKey(1); //1ms 이상 대기 해야 영상이 출력됨


		//가상키 https://m.blog.naver.com/postview.nhn?blogid=ikari13&logno=70087179768&proxyreferer=https%3a%2f%2fwww.google.com%2f
		if (_kbhit()) {
			
			//esc : 종료
			if (GetKeyState(VK_ESCAPE) & 0xff00) {
				std::cout << "-----< esc >-----";
				break;
			}
			
			else if(GetKeyState(0x41) & 0xff0) {
				if ((kik_flag == 1)) {
					kik_flag = 0;
					KIK;
				}
			}
			else {
				kik_flag = 1;
			}
			

		}
		//키보드 이벤트 종료


		//queryperformancecounter(&cpustop0);
		//cout << "프로세스 시간 = " << (float)(cpustop0.quadpart - cpustart0.quadpart) / (float)cpufreq.quadpart << "s" << endl;


	}//for(;;) END
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/*QueryPerformanceCounter(&cpuStop0);
	std::cout << "프로세스 시간 = " << (float)(cpuStop0.QuadPart - cpuStart0.QuadPart) / (float)cpuFreq.QuadPart << "s" << endl;
	std::cout << "사진 출력 횟수 = " << count << endl;
	std::cout << "프로세스 시간 = " << count << endl;*/

	return 0;
}

/*float time0 = 0.f;
LARGE_INTEGER cpuStart0, cpuStop0, cpuFreq;
QueryPerformanceFrequency(&cpuFreq); // 컴퓨터의 주파수 계산
QueryPerformanceCounter(&cpuStart0); // 시작 카운터 저장

함수 콜 ~

// time output ======================================== ========
QueryPerformanceCounter(&cpuStop0); // 종료 카운터 저장

// 경과 시간 출력
time0 = (float)(cpuStop0.QuadPart - cpuStart0.QuadPart) * 1 / (float)cpuFreq.QuadPart;
printf("프로세스 시간 = % lf 초 \ n", 시간 0);
sumTime + = time0;
*/
