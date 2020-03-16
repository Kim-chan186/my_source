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
#include <math.h>

////<수정 금지>//
//#define HUE_8B_MAX 180
//#define SAT_OR_VAL_8B_MAX 255
//
//#define HUE_8B_MIN 0
//#define SAT_OR_VAL_8B_MIN 0
//
//
////<수정 가능>//
//#define  HUE_GRAVITY         10
//#define  SATURATION_GRAVITY  80
//#define  VALUE_GRAVITY       90


//컴파일이 끝난 뒤 링크할떄 추가되는 파일
#pragma comment(lib, "winmm.lib")

#define TTT cout << "     Test" << endl;

using namespace cv;
using namespace cv::ml;
using namespace std;

void null_img(Mat& img, string str) {
	if (img.empty()){
		printf_s("%s, 이미지가 비어있습니다.tlqkf \n", str);
	}
}

Mat frame_ = imread("img_form_mp4/frame.PNG");
Mat bg_ = imread("img_form_mp4/bg_frame.png");
Mat bg2_ = imread("img_form_mp4/bg_frame2.png");
Mat light_ = imread("img_form_mp4/light_frame.PNG");
Mat img_buf_[3], img_show_;
Mat img_buf2_[4]; //순서, 채널
Mat img_buf3_[4];

const int	x1_ = 20 + 73 + 4,
			x2_ = 164 + 73 + 2,
			x3_ = 309 + 73 - 2,
			x4_ = 453 + 73 - 4,
			y_ = 154;
const int x_[4] = { x1_, x2_, x3_, x4_ }; // { 20 + 73 + 4, 73 + 2, 73 - 2, 73 - 4 };

Point p1_ = Point(x1_, y_);
Point p2_ = Point(x2_, y_);
Point p3_ = Point(x3_, y_);
Point p4_ = Point(x4_, y_);

double v_[4]; //gary
Vec3d g_[4]; //gain

void setunbackground() {
	cvtColor(light_, light_, COLOR_BGR2GRAY);
	Mat simg_buf_, simg_buf2_[3];
	for (int i = 0; i < 4; i++) {
		simg_buf_ = imread("img_form_mp4/light/gpart" + to_string(i + 1) + (string)".PNG", IMREAD_GRAYSCALE);
		
		/*simg_buf2_[0] = simg_buf_ * 0.6;
		simg_buf2_[1] = simg_buf_ * 1.2;
		simg_buf2_[2] = simg_buf_ * 1.2;

		merge(simg_buf2_, 3, simg_buf_);
		cvtColor(simg_buf_, simg_buf_, COLOR_BGR2HSV);
		
		split(simg_buf_, img_buf2_[i]);

		cvtColor(simg_buf_, simg_buf_, COLOR_GRAY2BGR);*/

		simg_buf_.convertTo(img_buf2_[i], CV_32FC3);
		//imshow("rr", img_buf2_[i]);
		//waitKey(1000);
	}

	int buf = x1_;

	for (int j = buf - 2; j <= buf + 2; j++) {
		v_[0] += light_.at<uchar>(y_, j);
	}
	buf = x2_;
	for (int j = buf - 2; j <= buf + 2; j++) {
		v_[1] += light_.at<uchar>(y_, j);
	}
	buf = x3_;
	for (int j = buf - 2; j <= buf + 2; j++) {
		v_[2] += light_.at<uchar>(y_, j);
	}
	buf = x4_;
	for (int j = buf - 2; j <= buf + 2; j++) {
		v_[3] += light_.at<uchar>(y_, j);
	}
}

void unbackground(Mat& img, Mat& show, int& flag) {
	
	Mat sample_ = img.clone() - bg_;
	Mat sample_hsv_;

	///cvtColor(sample_, sample_hsv_, COLOR_BGR2HSV);

	for (int i = 0; i < 3; i++) {
		int buf = x1_,
			sum[4] = { 0 };
		for (int j = buf - 2; j <= buf + 2; j++) {
			sum[0] += sample_.at<Vec3b>(y_, j)[i];
		}
		buf = x2_;
		for (int j = buf - 2; j <= buf + 2; j++) {
			sum[1] += sample_.at<Vec3b>(y_, j)[i];
		}
		buf = x3_;
		for (int j = buf - 2; j <= buf + 2; j++) {
			sum[2] += sample_.at<Vec3b>(y_, j)[i];
		}
		buf = x4_;
		for (int j = buf - 2; j <= buf + 2; j++) {
			sum[3] += sample_.at<Vec3b>(y_, j)[i];
		}
		for (int j = 0; j < 4; j++) {
			g_[j][i] = sum[j] / (v_[j]);
		}
		cout << g_[i] << endl;
	}//for_end
	
	for (int i = 0; i < 4; i++) { //4part
		for (int j = 0; j < 3; j++) { // 3채널
			int value;
			img_buf3_[j] = (img_buf2_[i] * g_[i][j]);//* 1.1) + ( (v_[i] / 5 - 170) / 10 * g_[i][j]);

			//cvtColor(img_buf3_[j], img_buf3_[j], COLOR_GRAY2BGR);
			//for (int iy = 0; iy < img_buf3_[j].rows; iy++) {
			//	float* bp = img_buf3_[j].ptr<float>(iy);
			//	for (int ix = 0; ix < img_buf3_[j].cols; ix++) {
			//		if (((v_[i] / 5 - 170) > 0) & (bp[ix] > 50)) {
			//			value = x_[i] - ix;
			//			if (value > 40) {//양수
			//				bp[ix] -= (value - 40) * 2;
			//			}
			//			else if (-40 > value) {//음수
			//				bp[ix] += (value + 40) * 2;
			//			}
			//		}
			//	}
			//}
			//cvtColor(img_buf3_[j], img_buf3_[j], COLOR_BGR2GRAY);
			if (i == 0) {
				img_buf3_[j].copyTo(img_buf_[j]);
			}
			else
			{
				hconcat(img_buf_[j], img_buf3_[j], img_buf_[j]);
			}
		}
	}//for_end
	cv:: merge(img_buf_, 3, img_show_);
	img_show_.convertTo(img_show_, CV_8UC3); 
	
	const int gain = 40;

	/*cvtColor(img_show_, img_show_, COLOR_BGR2HSV);
	for (int iy = 0; iy < img_show_.rows; iy++) {
		Vec3b* vp = img_show_.ptr<Vec3b>(iy);
		int m = 0;
		for (int ix = 0; ix < img_show_.cols; ix++) {
			int val = ix - x_[m];
			if (val < 0) {
				val *= -1;
			}
			if (val > gain){
				vp[ix][2] -= (val - gain) * 2;
			}
			if ((ix > 164) | (ix > 309) | (ix > 453)) {
				m++;
			}
		}
	}
	cvtColor(img_show_, img_show_, COLOR_HSV2BGR);
	///cvtColor(img_show_, img_show_, COLOR_HSV2BGR); 

	///////////////////////////////

	/*img_show_ = img_show_*0.7 - frame_ + bg_;
	img = (-img + img_show_);//+ bg_;*/

	/*img_show_ = img_show_ * 1.2 - frame_ + bg_;
	img = (img - img_show_);//+ bg_;
	show = img_show_.clone();*/
	if (flag < 100) {
		img_show_ = - frame_ + bg2_;
	}
	else 
		img_show_ = img_show_ - frame_ + bg_;
	img = img -(255 - frame_);
	img = img + img_show_; //+ bg_;
	show = img_show_.clone();

	////////////////////////////////
	
	show = img_show_ + bg_; 
	cv::imshow("img_show_", show);
	
	//y 95(153)~357
	//x  20~598 //579 // 144.5

	//20 164.5 309 453.5 598
}

/*
Mat img;
Mat train, label;
Ptr<KNearest> knn;
int k_value = 1;

void on_k_changed(int, void*);
void addPoint(const Point& pt, int cls);
void trainAndDisplay();

int main(void)
{
	img = Mat::zeros(Size(500, 500), CV_8UC3);
	knn = KNearest::create();

	namedWindow("knn");
	createTrackbar("k", "knn", &k_value, 5, on_k_changed);

	const int NUM = 30;
	Mat rn(NUM, 2, CV_32SC1);

	randn(rn, 0, 50);
	for (int i = 0; i < NUM; i++)
		addPoint(Point(rn.at<int>(i, 0) + 150, rn.at<int>(i, 1) + 150), 0);

	randn(rn, 0, 50);
	for (int i = 0; i < NUM; i++)
		addPoint(Point(rn.at<int>(i, 0) + 350, rn.at<int>(i, 1) + 150), 1);

	randn(rn, 0, 70);
	for (int i = 0; i < NUM; i++)
		addPoint(Point(rn.at<int>(i, 0) + 250, rn.at<int>(i, 1) + 400), 2);

	trainAndDisplay();

	waitKey();
	return 0;
}

void on_k_changed(int, void*)
{
	if (k_value < 1) k_value = 1;
	trainAndDisplay();
}

void addPoint(const Point& pt, int cls)
{
	Mat new_sample = (Mat_<float>(1, 2) << pt.x, pt.y);
	train.push_back(new_sample);

	Mat new_label = (Mat_<int>(1, 1) << cls);
	label.push_back(new_label);
}

void trainAndDisplay()
{
	knn->train(train, ROW_SAMPLE, label);

	for (int i = 0; i < img.rows; ++i) {
		for (int j = 0; j < img.cols; ++j) {
			Mat sample = (Mat_<float>(1, 2) << j, i);

			Mat res;
			knn->findNearest(sample, k_value, res);

			int response = cvRound(res.at<float>(0, 0));
			if (response == 0)
				img.at<Vec3b>(i, j) = Vec3b(128, 128, 255); // R
			else if (response == 1)
				img.at<Vec3b>(i, j) = Vec3b(128, 255, 128); // G
			else if (response == 2)
				img.at<Vec3b>(i, j) = Vec3b(255, 128, 128); // B
		}
	}

	for (int i = 0; i < train.rows; i++)
	{
		int x = cvRound(train.at<float>(i, 0));
		int y = cvRound(train.at<float>(i, 1));
		int l = label.at<int>(i, 0);

		if (l == 0)
			circle(img, Point(x, y), 5, Scalar(0, 0, 128), -1, LINE_AA);
		else if (l == 1)
			circle(img, Point(x, y), 5, Scalar(0, 128, 0), -1, LINE_AA);
		else if (l == 2)
			circle(img, Point(x, y), 5, Scalar(128, 0, 0), -1, LINE_AA);
	}

	imshow("knn", img);
}
*/






void Mouse_Event(int event, int x, int y, int flags, void* userdata)
{
	static bool flag = 0;
	static int x_ = 0, y_ = 0;
	if (event == EVENT_LBUTTONDOWN)
	{
		if (flag == 0) {
			printf(" %d %d", x, y);
			flag = 1;
			x_ = x;
			y_ = y;
		}
		else {
			flag = 0;
			printf(" %d %d", x - x_, y - y_);
			rectangle(*((Mat*)userdata), Point(x_, y_), Point(x, y), Scalar(255), 2);
		}
		//uint r = ((Mat*)userdata)->at<Vec3b>(y, x)[2];
		//uint g = ((Mat*)userdata)->at<Vec3b>(y, x)[1];
		//uint b = ((Mat*)userdata)->at<Vec3b>(y, x)[0];
		///포인터로 바꾸기
		// uchar __ = __.color.ptr<uchar>(x)
		// uchar __ = __.color.ptr<uchar>(y)
		///데이터 직접접근으로 바꾸기
		// uchar __ = __.data
	}
} 
//	cv::setMouseCallback("video", Mouse_Event, &video); //
void Mouse_Event2(int event, int x, int y, int flags, void* userdata)//with BGRA img
{
	static bool flag = 0;
	static int x_ = 0, y_ = 0;
	if (event == EVENT_LBUTTONDOWN)
	{
		if (flag == 0) {
			printf(" %d, %d", x, y);
			flag = 1;
			x_ = x;
			y_ = y;
		}
		else {
			flag = 0;
			printf(" %d %d", x - x_, y - y_);
			rectangle(*((Mat*)userdata), Point(x_, y_), Point(x, y), Scalar(0, 0, 0, 0), -1);
		}
		//uint r = ((Mat*)userdata)->at<Vec3b>(y, x)[2];
		//uint g = ((Mat*)userdata)->at<Vec3b>(y, x)[1];
		//uint b = ((Mat*)userdata)->at<Vec3b>(y, x)[0];
		///포인터로 바꾸기
		// uchar __ = __.color.ptr<uchar>(x)
		// uchar __ = __.color.ptr<uchar>(y)
		///데이터 직접접근으로 바꾸기
		// uchar __ = __.data
	}
}

void Mouse_Event3(int event, int x, int y, int flags, void* userdata)//with BGRA img
{
	static bool flag = 0;
	static int i = 0;
	static int x_ = 0, y_ = 0;
	if (event == EVENT_LBUTTONDOWN)
	{
		if (flag == 0) {
			printf(" %d, %d", x, y);
			flag = 1;
			x_ = x;
			y_ = y;
		}
		else {
			flag = 0;
			Mat show = (*(Mat*)userdata).clone();
			imwrite(to_string(i++ + 1) + (string)".PNG", (*(Mat*)userdata)(Range(y_, y), Range(x_, x)));
			
			rectangle(show, Point(x_, y_), Point(x, y), Scalar(0, 0, 0, 0), -1);
			cv::imshow("video", show);
		}
		//uint r = ((Mat*)userdata)->at<Vec3b>(y, x)[2];
		//uint g = ((Mat*)userdata)->at<Vec3b>(y, x)[1];
		//uint b = ((Mat*)userdata)->at<Vec3b>(y, x)[0];
		///포인터로 바꾸기
		// uchar __ = __.color.ptr<uchar>(x)
		// uchar __ = __.color.ptr<uchar>(y)
		///데이터 직접접근으로 바꾸기
		// uchar __ = __.data
	}
}
void Mouse_Event4(int event, int x, int y, int flags, void* userdata)//위치, 색출력
{
	if (event == EVENT_LBUTTONDOWN)
	{
		uint r = ((Mat*)userdata)->at<Vec3b>(y, x)[2];
		uint g = ((Mat*)userdata)->at<Vec3b>(y, x)[1];
		uint b = ((Mat*)userdata)->at<Vec3b>(y, x)[0];

		printf(" x,y : (%d, %d)\n bgr : (%d, %d, %d) \n \n ", x, y, b, g, r);
		///포인터로 바꾸기
		// uchar __ = __.color.ptr<uchar>(x)
		// uchar __ = __.color.ptr<uchar>(y)
		///데이터 직접접근으로 바꾸기
		// uchar __ = __.data
	}
	else if (event == EVENT_RBUTTONDOWN) {
		int h = 97;
		int w = 0;
		printf("@@@@@@@@@@@@@@");
		for (int i = 2; i >= 0; i--) {
			printf("\n");
			for (w = 0; w < 363; w += 4) {
				uint c = ((Mat*)userdata)->at<Vec3b>(h, w)[i];

				printf("%d\n", c);
			}
			printf("\n");
		}
	}
	//else if (event == EVENT_RBUTTONDOWN) {
	//	int h = 97;
	//	int w = 0;
	//	for (w = 0; w < 263; w += 4) {
	//		uint r = ((Mat*)userdata)->at<Vec3b>(h, w)[2];
	//		uint g = ((Mat*)userdata)->at<Vec3b>(h, w)[1];
	//		uint b = ((Mat*)userdata)->at<Vec3b>(h, w)[0];

	//		printf("%d, %d, %d\n", b, g, r);
	//	}
}

//클릭 지우개
void Mouse_Event5(int event, int x, int y, int flags, void* userdata)//위치, 색출력
{
	if ((event == EVENT_MOUSEMOVE) && (flags == EVENT_FLAG_LBUTTON ))
	{
		//uint r = ((Mat*)userdata)->at<Vec3b>(y, x)[2] = 0;
		//uint g = ((Mat*)userdata)->at<Vec3b>(y, x)[1] = 0;
		//uint b = ((Mat*)userdata)->at<Vec3b>(y, x)[0] = 0;

		//printf(" x,y : (%d, %d)\n bgr : (%d, %d, %d) \n \n ", x, y, b, g, r);
		circle((*(Mat*)userdata), Point(x, y), 5, Scalar::all(0), -1);
	}
	else if ((event == EVENT_MOUSEHWHEEL) && (flags == EVENT_FLAG_RBUTTON)) {
		
		circle((*(Mat*)userdata), Point(x, y), 10, Scalar::all(0), -1);
	}
}
//
//
//////cascade positives to negatives
//int main() {
//	int i = 1;
//	//Mat img = imread("img_form_mp4/학습3/" + to_string(i) + (string)".PNG");
//	
//	Mat img_buf		= imread("img_form_mp4/쉬움/쉬움" + to_string(i) + (string)".PNG");
//	Mat img_count	= Mat::zeros(Size(img_buf.cols, img_buf.rows), CV_8UC1);
//	Mat img			= Mat::zeros(Size(img_buf.cols, img_buf.rows), CV_32FC3);
//	Mat img_save	= Mat::zeros(Size(img_buf.cols, img_buf.rows), CV_32FC3);
//
//	namedWindow("video", WINDOW_AUTOSIZE);
//	cv::setMouseCallback("video", Mouse_Event5, &img); // 1~2, 5
//
//	double r=0
//		, g=0
//		, b=0;
//
//	for (i = 1; i <= 42; i++) {
//		img_buf = imread("img_form_mp4/쉬움/쉬움" + to_string(i) + (string)".PNG");
//
//		for (int y = 0; y < img_buf.rows; y++) {
//			double	* data_buf	= img_buf.ptr<double>(y),
//					* data		= img.ptr<double>(y),
//					* data_count = img_count.ptr<double>(y);
//
//			//cout << "img_buf.rows   " << img_buf.rows << endl;
//			imshow("video", img);
//			waitKey(1);
//			for (int x = 0; x < img_buf.cols * 3; x += 3) {
//				b = data_buf[x];
//				g = data_buf[x+1];
//				r = data_buf[x+2];
//
//				if ((r == 0) & (g == 0) & (b == 0) ) {
//				}
//				else {
//						//data[x] += b;
//						//data[x+1] += g;
//						//data[x+2] += r;
//						//printf("%d ", x/3);
//
//						//data_count[x/3] += 1;
//				}//if_end
//
//			}//x for_end
//			//cout << i << "+" << y << "line clear!!\n";
//		}
//		//cout << i << "Mat clear!!\n\n\n";
//	}//for_end
//
//	cout << "end add, start div \n";
//	for (int y = 0; y < img_buf.rows; y++) {
//		double* data = img.ptr<double>(y);
//
//		for (int x = 0; x < img_buf.cols; x++) {
//			for (int j = 0; j < 3; j++) {
//				if (img_count.at<Vec3b>(y, x)[0] != 0)
//					img_save.at<Vec3b>(y, x)[j] = *data++ / img_count.at<Vec3b>(y, x)[0];
//			}
//		}//x_for end
//	}//for_end
//
//	//imwrite("img_form_mp4/color_frame.PNG", img_save);
//	
//	while (1) {
//		imshow("video", img_save);
//		if (waitKey(1) == 32) {
//			//imwrite("img_form_mp4/학습3/" + to_string(i++) + (string)".PNG", img);
//			//img += imread("img_form_mp4/학습2/" + to_string(i) + (string)".PNG");
//		}
//	}
//
//}













///**/
////bg용 PNG만들기 Gray to BG_bgra
//int main() {
//	int i = 0;
//	Mat img = imread("img_form_mp4/쉬움/쉬움" + to_string(i + 1) + (string)".PNG");
//	imwrite("img_form_mp4/쉬움(확대)"
//		+ to_string(i++ + 1)
//		+ (string)".PNG"
//		, img);
//
//	printf("\nE:/cascade_data/positives5/%d.PNG", i);
//
//	//cvtColor(img, img, COLOR_BGR2BGRA);
//	resize(img, img, Size(620, 440), 0, 0, INTER_LINEAR);
//
//	namedWindow("video", WINDOW_AUTOSIZE);
//	cv::setMouseCallback("video", Mouse_Event, &img); // 1~3, 5
//	
//
//	Size size = Size((int)620, (int)440);
//	VideoWriter writer, writer1;
//	double fps = 60.0;
//
//	writer.open("비올레타_쉬움(확대).avi", VideoWriter::fourcc('M', 'J', 'P', 'G'), fps, size, true);
//	writer1.open("비올레타_쉬움(빠름)(스킵).avi", VideoWriter::fourcc('M', 'J', 'P', 'G'), fps, size, true);
//
//	while (1) {
//		char ch = waitKey(1);
//
//		if (1) {//ch == 32) {
//			string str2 = "img_form_mp4/쉬움/쉬움" + to_string(i + 1) + (string)".PNG";
//			if (img.empty()) {
//				break;
//			}
//			img = imread(str2);
//			resize(img, img, Size(620, 440), 0, 0, INTER_LINEAR);
//
//			printf("\nE:/cascade_data/positives5/%d.PNG", i);
//
//			imwrite("img_form_mp4/쉬움(확대)/"
//				+ to_string(i++ + 1)
//				+ (string)".PNG"
//				, img);
//
//
//
//			if (!writer.isOpened())
//			{
//				cout << "동영상을 저장하기 위한 초기화 작업 중 에러 발생" << endl;
//				return 1;
//			}
//			writer.write(img);
//			if (i % 4 == 0) {
//				writer1.write(img);
//			}
//			if (img.empty()) {
//				break;
//			}
//		}//if( ==' ')_end
//		
//
//
//		imshow("video", img);
//	}
//	
//}
const double	theta = 45,
				pi = 3.141592;

const double	sin_j = sin(theta * pi / 180),
				cos_i = cos(theta * pi / 180),
				tan_ij = tan(theta * pi / 180);

const int		i_start = 40,
				j_start = 400,
				img_cols = 620,
				img_row = 440;
void xyz2ijk_codinate(Vec3d v, Vec2d &v_out) {
	v_out = Vec2d(i_start + v[0] + v[1] * cos_i, j_start - v[1] * sin_j - v[2]);
	return;
}
void xyz2ijk_codinate(double x, double y, double z, Vec2d &v_out) {
	xyz2ijk_codinate(Vec3d(x, y, z), v_out);
}
void xyz2ijk_codinate(double x, double y, double z, double& x_out, double& y_out) {
	Vec2d v_;
	xyz2ijk_codinate(Vec3d(x, y, z), v_);
	x_out = v_[0];
	y_out = v_[1];
}


void make_BG(Mat& img_) {
	line(img_, Point(i_start, 0), Point(i_start, img_row), Scalar(120, 10, 10), 3);
	line(img_, Point(i_start, 0), Point(i_start, img_row), Scalar(150, 20, 20), 2);//i

	line(img_, Point(0, j_start), Point(img_cols, j_start), Scalar(10, 120, 10), 3);
	line(img_, Point(0, j_start), Point(img_cols, j_start), Scalar(20, 150, 20), 2);//z

	line(img_, Point(i_start, j_start), Point(img_cols, j_start - (img_cols - i_start) * tan_ij), Scalar(10, 10, 120), 3);
	line(img_, Point(i_start, j_start), Point(img_cols, j_start - (img_cols - i_start) * tan_ij), Scalar(20, 20, 150), 2);//j
	cout << tan_ij << " " << cos_i << " " << (img_cols - i_start);
}
void throw_circle(Point p, Mat& img_, Scalar sca = Scalar::all(255),int r = 1) {
	circle(img_, p, r, sca, -1);
}
void throw_circle(int p1, int p2, Mat& img_, Scalar sca = Scalar::all(255), int r = 1) {
	throw_circle(Point(p1, p2), img_, r);
}
void throw_circle(Vec2d v2d, Mat& img_, Scalar sca = Scalar::all(255), int r = 1) {
	throw_circle(Point(v2d[0], v2d[1]), img_, r);
}
/*
int main() {
	Mat graph = Mat::zeros(Size(640, 440), CV_8UC3) + 20;
	make_BG(graph);
	Vec2d v2d;
	Vec3d v3d;
	// 선언 /////////////////////////////////

	while (1) 
	{

	}
	

	for (int m  = 0; m < 100; m++) {
		for (int g = 0; g < 100; g++) {
			for (int h = 0; h < 100; h++) {

				v3d = Vec3d(m * 10, 300, h * 10);
				xyz2ijk_codinate(v3d, v2d);
				throw_circle(v2d, graph, Scalar(255, 20, 20));

				v3d = Vec3d(m * 10, g * 10, 200);
				xyz2ijk_codinate(v3d, v2d);
				throw_circle(v2d, graph, Scalar(0, 255, 0));

				v3d = Vec3d(m * 10, g * 10, 100);
				xyz2ijk_codinate(v3d, v2d);
				throw_circle(v2d, graph, Scalar(0, 0, 255));
			}
		}
	}
	

	for (char c = 0; c != 27; c = waitKey(1)) {
		// 출력 /////////////////////////////////
		graph.convertTo(graph, CV_8UC3);
		imshow("graph", graph);
	}
}

*/





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


using namespace cv;
using namespace std;



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
		if (track_vaule == 0) {
			track_vaule = 1;
			return;
		}
		track_vaule = (*(int*)userdata);
	}
};

void Mouse_Event22(int event, int x, int y, int flags, void* userdata)
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
	return pow((p1.x - p2.x), 2) + pow((p1.y - p2.y), 2);
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












int main() {

	////////////////////////////////////////////

	int range_count = 0;         //일반 변수

	Scalar red(0, 0, 255);         //색상 지정
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

	VideoCapture cap(0);    //0번 카메라(노트북 웹캠)을 사용
					  //현시간에 대한 영상을 cpa객체로부터 가져올 수 있음
	//Mat img_frame;       //클래스 사용안되서 전역으로 처리함

	Mat img_hsv;         //이미지를 저장할 객체 2개 선언

	if (!cap.isOpened()) {
		cerr << "ERROR! Unable to open camera\n";
		return -1;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	Mat img_mask1;
	Mat img_mask2;

	Point point_past[2] = { Point(0,0),Point(420,0) };
	Point point[2];


	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



	bool kik_flag = 1;
	int count = 0;         // 반복 횟수

	for (;;) {
		count++;

		//cap.read(img_mask1);


		cap >> cpm_img;// = imread("물고기.jpg");

		//flip(img_mask1, cpm_img, 1);

		// check if we succeeded
		if (cpm_img.empty()) {
			cerr << "ERROR! blank frame grabbed\n";
			return 0;
		}


		cvtColor(cpm_img, img_hsv, COLOR_BGR2HSV);
		//지정한 hsv 범위를 이용하여 영상을 이진화

		inRange(img_hsv, pa->Low_Scalar(), pa->High_Scalar(), img_mask1);
		if (pa->get_key()) {
			inRange(img_hsv, pa->Low_Scalar(true), pa->High_Scalar(true), img_mask2);
			img_mask1 |= img_mask2;
		}///이진화를 할 색상이 영역이 2개의 부분으로 지정된 경우(빨간색) 2번째 영역에대한 이진화를 해 더함
		//검정부분의 값이 0임으로 비트연산으로 더하면흰 부분의 색상이 겹침




		////morphological opening 작은 점들을 제거(수축)
		//erode(img_mask1, img_mask1, getStructuringElement(MORPH_ELLIPSE,
		//	Size(Variable::track4, Variable::track4)));

		//dilate(img_mask1, img_mask1, getStructuringElement(MORPH_ELLIPSE,
		//	Size(Variable::track4 + Variable::track5, Variable::track4)));


		////morphological closing 영역의 구멍 메우기(팽창)
		//dilate(img_mask1, img_mask1, getStructuringElement(MORPH_ELLIPSE,
		//	Size(Variable::track5, Variable::track5)));

		//erode(img_mask1, img_mask1, getStructuringElement(MORPH_ELLIPSE,
		//	Size(Variable::track5, Variable::track5)));


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
		/*
		Mat img_show = imread("물고기.PNG");

		cvtColor(img_mask1 + 70, img_mask1, COLOR_GRAY2BGR);
		img_show = ~(~img_mask1 + ~img_show);
		*/

		//윈도우 생성
		cv::namedWindow("이진화 영상", WINDOW_AUTOSIZE);
		cv::namedWindow("원본 영상", WINDOW_AUTOSIZE);
		cv::namedWindow("트렉바", WINDOW_AUTOSIZE);


		//윈도우 출력
		cv::imshow("이진화 영상", img_mask1);
		cv::imshow("원본 영상", cpm_img);
		//cv::imshow("show 영상", img_show);


		//"트렉바"윈도우에 트렉바 생성, 이벤트 등록
		cv::createTrackbar("hue :", "트렉바", &Variable::track1, 90, On_track::track1, pa);
		cv::createTrackbar("sat :", "트렉바", &Variable::track2, 255, On_track::track2, pa);
		cv::createTrackbar("val :", "트렉바", &Variable::track3, 255, On_track::track3, pa);
		cv::createTrackbar("수축 :", "트렉바", &Variable::track4, 20, On_track::track4, pa);
		cv::createTrackbar("팽창 :", "트렉바", &Variable::track5, 20, On_track::track5, pa);
		//0일때 죽는거 수정 해야함


		//윈도우에 콜백함수를 등록
		cv::setMouseCallback("원본 영상", Mouse_Event22, pa);

		cv::waitKey(1); //1ms 이상 대기 해야 영상이 출력됨

		//키보드 이벤트 종료

	}//for(;;) END
	return 0;
}











//
//
//
////흑백 캠
//int main()
//{
//	Mat img_color;
//
//	//비디오 캡쳐 초기화
//	VideoCapture cap(0);
//	if (!cap.isOpened()) {
//		cerr << "에러 - 카메라를 열 수 없습니다.\n";
//		return -1;
//	}
//
//	// 동영상 파일을 저장하기 위한 준비  
//	Size size = Size((int)cap.get(CAP_PROP_FRAME_WIDTH),
//		(int)cap.get(CAP_PROP_FRAME_HEIGHT));
//
//	VideoWriter writer;
//	double fps = 30.0;
//	writer.open("output.avi", VideoWriter::fourcc('M', 'J', 'P', 'G'), fps, size, true);
//	if (!writer.isOpened())
//	{
//		cout << "동영상을 저장하기 위한 초기화 작업 중 에러 발생" << endl;
//		return 1;
//	}
//
//
//	while (1)
//	{
//		cap.read(img_color);
//		if (img_color.empty()) {
//			cerr << "빈 영상이 캡쳐되었습니다.\n";
//			break;
//		}
//		//동영상 파일에 한 프레임을 저장함.  
//		writer.write(img_color);
//
//		imshow("Color", img_color);
//
//		if (waitKey(25) >= 0)
//			break;
//	}
//	return 0;
//}


////
//////흑백 동영상, 동영상 편집
//int main()
//{
//	Mat frame, show, show2, video = imread("img_form_mp4/배경frame2.PNG");
//	Mat img1;// = imread("img_form_mp4/배경frame.PNG");
//	Mat img2 = imread("img_form_mp4/배경frame2.PNG");
//	Mat img3 = imread("img_form_mp4/배경frame3.PNG");
//	frame = img1 = (img2 * 0.3 - 10) + img3;
//
//	//비디오 캡쳐 초기화
//	VideoCapture cap("비올레타_어려움.mp4");
//	if (!cap.isOpened()) {
//		cerr << "에러 - 카메라를 열 수 없습니다.\n";
//		return -1;
//	}
//
//	//동영상 파일을 저장하기 위한 준비  
//	Size size = Size((int)cap.get(CAP_PROP_FRAME_WIDTH),
//					 (int)cap.get(CAP_PROP_FRAME_HEIGHT)*2);
//
//	VideoWriter writer, writer1;
//	double fps = 60.0;
//	writer.open("비올레타_어려움(unbackgound).avi", VideoWriter::fourcc('M', 'J', 'P', 'G'), fps, size, true);
//	writer1.open("비올레타_어려움(unbackgound)(스킵).avi", VideoWriter::fourcc('M', 'J', 'P', 'G'), fps, size, true);
//
//
//	if (!writer.isOpened())
//	{
//		cout << "동영상을 저장하기 위한 초기화 작업 중 에러 발생" << endl;
//		return 1;
//	}
//	namedWindow("video", 1);
//	cv::setMouseCallback("video", Mouse_Event, &video); //
//	setunbackground();
//	
//	for (int i = 0;;)
//	{ 
//		if (waitKey(1) == 32) {
//		frame = imread("img_form_mp4/어려움/어려움"
//				+ to_string(i + 1)
//				+ (string)".PNG"
//				);
//			printf("\n %d ", i+1);
//			if (frame.empty()) {
//				cerr << "빈 영상입니다.\n";
//				break;
//			}
//
//			video = frame.clone();
//			unbackground(frame, show, i);
//			vconcat(frame, video, frame);
//			writer.write(frame);
//
//			/*imwrite("img_form_mp4/unbackgound/"
//				+ to_string(i + 1)
//				+ (string)".PNG"
//				, frame);
//			imwrite("img_form_mp4/backgound/"
//				+ to_string(i + 1)
//				+ (string)".PNG"
//				, show);*/
//
//			if ((i > 120)&(i % 4 == 0)) {
//				writer1.write(frame);
//			}
//			i++;
//			
//		}
//
//		
//		
//		//cvtColor(frame, frame, COLOR_BGR2GRAY);
//		//cvtColor(frame, frame, COLOR_GRAY2BGR);
//		//frame -= (img1);
//		
//
//		cv::imshow("video", frame);
//	}
//	return 0;
//}



//make bg
//int main() {
//
//	int i = 0;
//	Mat img = Mat::zeros(1, 2, CV_8U),
//		img_buf = Mat::zeros(1, 2, CV_8U),
//		img_show = Mat::zeros(1, 2, CV_8U);
//
//	namedWindow("video", WINDOW_AUTOSIZE);
//	cv::setMouseCallback("video", Mouse_Event2, &img);
//
//	img = imread("bg/" + to_string(i++ + 1) + (string)".PNG", IMREAD_UNCHANGED);
//	img_buf = imread("bg/" + to_string(i++ + 1) + (string)".PNG", IMREAD_UNCHANGED);
//	cvtColor(img, img, COLOR_BGR2BGRA);
//	cvtColor(img_buf, img_buf, COLOR_BGR2BGRA);
//	while (1) {
//		hconcat(img, img_buf, img_show);
//		imshow("video", img_show);
//
//		char ch = waitKey(1);
//		if (ch == 32) {
//			for (int i = 0; i < 16; i++)
//			{
//				img_buf = imread("bg/" + to_string(i + 1) + (string)".PNG", IMREAD_UNCHANGED);
//				cvtColor(img_buf, img_buf, COLOR_BGR2BGRA);
//				for (int j = 0; j < img.cols; j++) {
//					for (int k = 0; k < img.rows; k++) {
//						if (img_buf.at<Vec4b>(k, j)[3] == 0) {
//						}
//						else if (img.at<Vec4b>(k, j)[3] == 0) {
//							img.at<Vec4b>(k, j)[0] = img_buf.at<Vec4b>(k, j)[0];
//							img.at<Vec4b>(k, j)[1] = img_buf.at<Vec4b>(k, j)[1];
//							img.at<Vec4b>(k, j)[2] = img_buf.at<Vec4b>(k, j)[2];
//						}
//						else {
//							img.at<Vec4b>(k, j)[0] = (img.at<Vec4b>(k, j)[0] + img_buf.at<Vec4b>(k, j)[0]) / 2;
//							img.at<Vec4b>(k, j)[1] = (img.at<Vec4b>(k, j)[1] + img_buf.at<Vec4b>(k, j)[1]) / 2;
//							img.at<Vec4b>(k, j)[2] = (img.at<Vec4b>(k, j)[2] + img_buf.at<Vec4b>(k, j)[2]) / 2;
//						}
//						img_buf.at<Vec4b>(k, j)[0] /= 2;
//						img_buf.at<Vec4b>(k, j)[1] /= 2;
//						img_buf.at<Vec4b>(k, j)[2] /= 2;
//					}
//					if (j % (int)(1 + (i / 4)) == 0) {
//						hconcat(img, img_buf, img_show);
//						imshow("video", img_show);
//						waitKey(1);
//					}
//				}
//			}
//		}
//		else if (ch == 's') {
//			cvtColor(img, img, COLOR_BGRA2BGR);
//			cout << "save!!wn";
//			imwrite("bg/bg.PNG", img);
//		}
//		else if (ch == 'a') {
//
//		}
//	}
//}

	
//
////기본 이미지 geto
//int main() {
//	Mat bg = imread("img_form_mp4/bg_frame2.png");
//	Mat frame = imread("img_form_mp4/frame.PNG");
//	Mat light = imread("img_form_mp4/light_frame2.PNG");
//	Mat test = imread("img_form_mp4/컬러배경sample.PNG");
//	Mat sample = imread("img_form_mp4/학습/학습 (15).PNG");// - bg;
//	Mat img, show;
//
//	namedWindow("video", 1);
//	namedWindow("video2", 1);
//
//	//imwrite("img_form_mp4/배경frame2.PNG", test + ( 255 - frame) - (255 - frame) + bg);
//
//	bg *= 0.29;
//	imwrite("img_form_mp4/bg_frame.PNG", bg);
//	
//	cout << light.cols << "  " << light.rows << endl;
//	cout << bg.cols << "  " << bg.rows << endl;
//	light -= bg;
//	imwrite("img_form_mp4/light_frame.PNG", light);
//	
//	/*for (int i = 0; ; i++) {
//		bg.convertTo(bg, CV_32FC3);
//		sample.convertTo(sample, CV_32FC3);
//		show =  sample - (bg * 0.01*i);
//		show.convertTo(show, CV_8UC3);
//		
//		imshow("video", show);
//		imwrite("img_form_mp4/bg/i"
//			+ to_string(i + 1)
//			+ "j0.PNG"
//			, show*2);
//		imwrite("light/1.PNG", show);
//		show += Scalar::all(50);
//		imshow("video", show);
//		imwrite("img_form_mp4/bg/i"
//			+ to_string(i + 1)
//			+ "j50.PNG"
//			, show*2);
//		imwrite("light/1.PNG", show);
//
//		
//		show = (bg * 0.01*i) - sample;
//		show.convertTo(show, CV_8UC3);
//		imshow("video", show);
//		imwrite("img_form_mp4/-bg/i"
//			+ to_string(i + 1)
//			+ "j0.PNG"
//			, show*2);
//		imwrite("light/1.PNG", show);
//		show += Scalar::all(50);
//		imshow("video", show);
//		imwrite("img_form_mp4/-bg/i"
//			+ to_string(i + 1)
//			+ "j50.PNG"
//			, show*2);
//		imwrite("light/1.PNG", show);
//		waitKey(1);
//	}*/
//	
//
//	imwrite("img_form_mp4/light/1.PNG", light(Rect( 20, 95, 145, 357 - 95)));
//	imwrite("img_form_mp4/light/2.PNG", light(Rect(164, 95, 145, 357 - 95)));
//	imwrite("img_form_mp4/light/3.PNG", light(Rect(309, 95, 145, 357 - 95)));
//	imwrite("img_form_mp4/light/4.PNG", light(Rect(453, 95, 145, 357 - 95)));
//	//VideoCapture cap("비올레타_쉬움.mp4");
//	/*if (!cap.isOpened())
//	{
//		printf("동영상 파일을 열수 없습니다. \n");
//	}*/
//	//동영상 플레이시 크기를  320x240으로 지정  
//	//cap.set(CAP_PROP_FRAME_WIDTH, 1140);
//	//cap.set(CAP_PROP_FRAME_HEIGHT, 536);
//
//	//Mat frame;
//	//cap >> frame;
//	//img = (bg + light);// -sample;
//	
//	
//
//	int x1 = 20 + 73 + 4,
//		x2 = 164 + 73 + 2,
//		x3 = 309 + 73 - 2,
//		x4 = 453 + 73 - 4;
//	int y = 154;
//
//	Point p1 = Point(x1, y);
//	Point p2 = Point(x2, y);
//	Point p3 = Point(x3, y);
//	Point p4 = Point(x4, y);
//
//	//circle(light, p1, 5, Scalar::all(255));
//	//circle(light, p2, 5, Scalar::all(255));
//	//circle(light, p3, 5, Scalar::all(255));
//	//circle(light, p4, 5, Scalar::all(255));
//
//	Vec3d v1, v2, v3, v4; //gary
//	Vec3d l1, l2, l3, l4; //만들어야될 조명
//	Vec3d g[4]; //gain
//
//	
//	cvtColor(light, light, COLOR_BGR2GRAY);
//	imshow("video2", light);
//
//	for (int i = 0; i < 1; i++) {
//		int buf = x1,
//			sum[4] = { 0 };
//		for (int j = buf - 2; j <= buf + 2; j++) {
//			sum[0] += light.at<uchar>(y, j);
//
//			cout << sum[0] << "sum" << endl;
//		}
//		buf = x2;
//		for (int j = buf - 2; j <= buf + 2; j++) {
//			sum[1] += light.at<uchar>(y, j);
//		}
//		buf = x3;
//		for (int j = buf - 2; j <= buf + 2; j++) {
//			sum[2] += light.at<uchar>(y, j);
//		}
//		buf = x4;
//		for (int j = buf - 2; j <= buf + 2; j++) {
//			sum[3] += light.at<uchar>(y, j);
//		}
//		v1[1] = sum[0] / 5;
//		v2[1] = sum[1] / 5;
//		v3[1] = sum[2] / 5;
//		v4[1] = sum[3] / 5;
//
//		cout << v1 << v2 << v3 << v4 << endl;
//	}
//	for (int i = 0; i < 3; i++) {
//		int buf = x1,
//			sum[4] = { 0 };
//		for (int j = buf - 2; j <= buf + 2; j++) {
//			sum[0] += sample.at<Vec3b>(y, j)[i];
//		}
//		buf = x2;
//		for (int j = buf - 2; j <= buf + 2; j++) {
//			sum[1] += sample.at<Vec3b>(y, j)[i];
//		}
//		buf = x3;
//		for (int j = buf - 2; j <= buf + 2; j++) {
//			sum[2] += sample.at<Vec3b>(y, j)[i];
//		}
//		buf = x4;
//		for (int j = buf - 2; j <= buf + 2; j++) {
//			sum[3] += sample.at<Vec3b>(y, j)[i];
//		}
//		l1[i] = sum[0] / 5;
//		l2[i] = sum[1] / 5;
//		l3[i] = sum[2] / 5;
//		l4[i] = sum[3] / 5;
//
//		cout << l1 <<l2 << l3 << l4 << endl;
//	}
//	for (int i = 0; i < 3; i++) {
//		g[0][i] = l1[i] / v1[1];
//		g[1][i] = l2[i] / v2[1];
//		g[2][i] = l3[i] / v3[1];
//		g[3][i] = l4[i] / v4[1];
//
//		cout << g[0] << g[1] << g[2] << g[3] << endl;
//	}
//
//
//	//cvtColor(light, light, COLOR_GRAY2BGR);
//
//	/*Mat light_clone[4];
//	cv::setMouseCallback("video", Mouse_Event4, light_clone); // 4
//	TTT
//	for (int i = 0; i < 4; i++) {
//		light_clone[i] = Mat::zeros(Size(620, 440), CV_8UC3);//(light.clone()).setTo(Scalar(0)); //Mat::zeros(Size(light.cols, light.rows), CV_8UC1);
//	}
//	
//	TTT
//	for (int x = 20, i = 0; x < 620; x++) {
//		for (int y = 0; y < 440; y++) {
//			for (int h = 0; h < 3; h++) {
//				light_clone[i].at<Vec3b>(y, x)[h] = light.at<Vec3b>(y, x)[h];
//			}
//		}	//uint r = light.at<Vec3b>(y, x)[2];
//		//cout << x << " ";
//		imshow("video", light_clone[i]);
//		waitKey(1);
//		if ((x == 165) | (x == 306) | (x == 452)) {
//			cout << "+";            
//			i++;
//		}
//			
//	}*/
//	//circle(light_clone[0], p1, 5, Scalar::all(255), -1);
//	//circle(light_clone[0], p2, 5, Scalar::all(255), -1);
//	//circle(light_clone[0], p3, 5, Scalar::all(255), -1);
//	//circle(light_clone[0], p4, 5, Scalar::all(255), -1);
//	TTT
//		/*imwrite("img_form_mp4/light/light1.PNG", light_clone[0]);
//		imwrite("img_form_mp4/light/light2.PNG", light_clone[1]);
//		imwrite("img_form_mp4/light/light3.PNG", light_clone[2]);
//		imwrite("img_form_mp4/light/light4.PNG", light_clone[3]);*/
//	imwrite("img_form_mp4/light/gpart1.PNG", light(Rect(0, 0, 166, 440)));
//	imwrite("img_form_mp4/light/gpart2.PNG", light(Rect(166, 0, 307-166, 440)));
//	imwrite("img_form_mp4/light/gpart3.PNG", light(Rect(307, 0, 453-307, 440)));
//	imwrite("img_form_mp4/light/gpart4.PNG", light(Rect(453, 0, 620-453, 440)));
//
//	imshow("video", light);
//
//	Mat img_buf[3], img_show;
//	Mat img_buf2[4];
//	Mat img_buf3[4];
//
//	TTT
//
//	for(int i = 0; i < 4; i++) { //4part
//		cout << "\n\n " << i << " line ";
//		img_buf2[i] = imread("img_form_mp4/light/gpart" + to_string(i + 1) + (string)".PNG", IMREAD_GRAYSCALE);
//		img_buf2[i].convertTo(img_buf2[i], CV_32FC1);
//		null_img(img_buf2[i], "img_buf2[i]");
//		imshow("video2", img_buf2[i]);
//		waitKey(1);
//		
//
//		for (int j = 0; j < 3; j++) { // 3채널
//
//			
//			img_buf3[j] = img_buf2[i] *g[i][j];
//			null_img(img_buf3[j], "\>\>img_buf[i]3\<\<");
//			cout << j << "j ";
//			
//
//			if (i == 0) {
//				img_buf3[j].copyTo(img_buf[j]);
//			}
//			else
//			{
//				hconcat(img_buf[j], img_buf3[j], img_buf[j]);
//			}
//			null_img(img_buf[j], "img_buf[j\i]");
//			imshow("video", img_buf[j]);
//			waitKey(1);
//		}
//		
//	}
//	TTT
//	merge(img_buf, 3, img_show);
//	waitKey(1);
//	img_show.convertTo(img_show, CV_8UC3);
//	for (int i = 0;;i++)
//	{
//		//웹캡으로부터 한 프레임을 읽어옴  
//		
//		char c = waitKey(5);
//		//30ms 정도 대기하도록 해야 동영상이 너무 빨리 재생되지 않음.  
//		if (c == 27) break; //ESC키 누르면 종료  
//		else if (c == 32) {
//			//cap >> frame;
//		}
//		
//		imshow("video", img_show);
//		imwrite("img_form_mp4/light/sample.PNG", img_show);
//		/*imwrite("img_form_mp4/조명/"
//			+ to_string(i + 1)
//			+ (string)".PNG"
//			, img2);
//		*/
//	}
//	//y 95(153)~357
//	//x  20~598 //579 // 144.5
//
//	//20 164.5 309 453.5 598
//}


////배경 frame 제거
//int main() {
//	Mat bg_frame = imread("img_form_mp4/배경frame.PNG");
//	Mat img;
//
//	namedWindow("video", 1);
//	cv::setMouseCallback("video", Mouse_Event4, &img); // 
//
//	for (int i = 0; i<42; i++)
//	{
//		Mat img = imread("img_form_mp4/학습/학습 ("
//			+ to_string(i + 1)
//			+ (string)").PNG"
//			);
//
//		imwrite("img_form_mp4/학습2/"
//			+ to_string(i + 1)
//			+ (string)".PNG"
//			,img - bg_frame
//			);
//		
//		
//		char c = waitKey(50);
//		if (c == 27) break; //ESC키 누르면 종료  
//		else if (c == 32) {
//		}
//		//cvtColor(frame, frame, COLOR_BGR2GRAY);
//		imshow("video", bg_frame);
//	}
//}







////이미지 합치기, 조명+배경 프레임 만들기
//int main() {
//	const int page = 42;
//	Mat //Arr = Mat(Size(620, 440), CV_8UC1, 10).setTo(Scalar(page)),
//		img = Mat::zeros(Size(620, 440), CV_8UC3),
//		img2 = Mat::zeros(Size(620, 440), CV_8UC4),
//		img_data[page];
//
//	int data0[page] = { 0 },
//		data1[page] = { 0 },
//		data2[page] = { 0 };
//
//	for (int i = 0; i < page; i++) {
//		/*resize(		imread("PNG/avg/비올레타_avg_data" 
//					+ to_string(i + 1) 
//					+ (string)".PNG"),
//			img_data[i], 
//			Size(1140, 536)
//		);*/
//		//img += img_data[i] / page;
//		//cvtColor(img_data[i], img_bgra[i], COLOR_BGR2BGRA);
//		img_data[i] = imread("img_form_mp4/학습3/"
//						+ to_string(i + 1)
//						+ (string)".PNG");
//		
//		imshow("vidio", img_data[i]);
//		waitKeyEx(1);
//	}
//	int flag = 0;
//	for (int x = 0; x < 620; x++, flag++) {
//		//waitKeyEx(1);
//		//imshow("vidio", img);
//		for (int y = 0; y < 440;  y++) {
//
//			double	sum0 = 0,
//					sum1 = 0,
//					sum2 = 0;
//			int count = 0;
//
//			for (int i = 0; i < page; i++) {
//				data0[i] = img_data[i].at<Vec3b>(y, x)[0];
//				data1[i] = img_data[i].at<Vec3b>(y, x)[1];
//				data2[i] = img_data[i].at<Vec3b>(y, x)[2];
//			}
//			
//			for (int i = 0; i < page; i++) {
//				if((data0[i] == 0)& (data1[i] == 0)& (data2[i] == 0)){
//				}
//				else {
//					count++;
//					sum0 += data0[i];
//					sum1 += data1[i];
//					sum2 += data2[i];
//				}
//			}
//			img.at<Vec3b>(y, x)[0] = sum0 / count;
//			img.at<Vec3b>(y, x)[1] = sum1 / count;
//			img.at<Vec3b>(y, x)[2] = sum2 / count;
//
//		}//for_end
//	}
//	cv::imwrite("img_form_mp4/light_fram2.PNG", img);
//	for (int i =0; 0; i++) {
//		waitKeyEx(1);
//		imshow("vidio", img);
//		cv::imwrite("img_form_mp4/light_fram2.PNG", img);
//
//
//		Mat frame = imread("img_form_mp4/배경frame2.png") - imread("img_form_mp4/배경frame3.PNG");
//		Mat frame2;
//		Mat light = imread("img_form_mp4/light_fram.PNG");
//
//		frame2 = light - frame * (0.01*i);
//		imshow("video", frame2);
//		imshow("video2", light);
//
//		imwrite("img_form_mp4/조명/"
//			+ to_string(i + 1)
//			+ (string)".PNG"
//			, frame2);
//	}
//}