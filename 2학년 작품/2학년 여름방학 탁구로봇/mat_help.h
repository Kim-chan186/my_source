#pragma once
#ifndef MAT_HELP_H
#define MAT_HELP_H

#include <opencv2/opencv.hpp>

#define MINAREA				0				//무시할 영역크기

#define org		Scalar(76, 81, 164)		//색상 지정 ball
#define org0	Scalar(90, 97, 180)

#define A1		50.38
#define A2		16.0	//
#define B1		432.33
#define B2		16.0	//
#define R1		-60.34	// -100.50
#define R2		650.0	// 880 - in_y
#define D1		536.5	// 573.94
#define D2		650.0	// 880 - in_y

#define Lmax	165.168		//	line_max
#define Lmaxp	443.7/20		//	line_max by pixel div 20
#define sinB	0.93606	//  sin(69.527720481)
#define H2cosB	118.57		//	0.484096 2HcosB 
#define H2cosBp	318.5		//  Hp 
#define ConsH	168.5		//	constant camera H position for z = 0 to camera
#define ConsHp	452.67		//
#define ConsIN	38.0		//	constant camera x position correction(x parameter start point)
#define ConsZ	3.0			//  z correction

#define ConsH0	188.5	//
#define ConsIN0	140.3

#define CWS		3.5		//	constant width start == (x=0)
#define CWL		146.0	//152.5 - (3+3.5)		//	_Line
#define CHS		9.6		//	_Height == (y=0) 
#define CHL		260.7	//274 - (3.7+9.6)		//	_Line

	
//템플릿으로 만들어야 된는데 귀찮다...
namespace cv {

	//연산자 오버로딩
	Point operator + (const Point& p1, const int& n) {
		Point p = p1;
		p.x += n;
		p.y += n;
		return p;
	}
	Point operator - (const Point& p1, const int& n) {
		Point p = p1;
		p.x -= n;
		p.y -= n;
		return p;
	}
	Scalar operator * (const Scalar& s, const int& n) {
		return Scalar(s.val[0] * n, s.val[1] * n, s.val[2] * n);
	}

	void throw_ball(int flag = 0, Mat* mat = nullptr) {
		static int num = 0;

		if (flag == 1) {
			num = 1;
		}
		else if (num == 0) {
			return;
		}
		else if (num == 48) {
			num = 0;
		}
		else if (num <= 30) {
			circle(*mat, Point(800 - 17 * num, 400 - 8 * num), 13 - num * num / 235.0,
				Scalar(1, 150, 240), -1);
			num++;
		}if (num > 30) {
			circle(*mat, Point(800 - 17 * num, 400 - 8 * num), 6 + (num - 28) / 3,
				Scalar(1, 150, 240), -1);
			num++;
		}

	}






	void drawing_auxiliary_line(Mat img, int width, int height) {//rs
		line(img,
			Point(width / 4, 0),
			Point(width / 4, height),
			Scalar(255, 255, 255), 1);
		line(img,
			Point(width * 3 / 4, 0),
			Point(width * 3 / 4, height),
			Scalar(255, 255, 255), 1);
		line(img,
			Point(0, height / 2),
			Point(width, height / 2),
			Scalar(255, 255, 255), 1);
	}
	void drawing_auxiliary_line(Mat img1, Mat img0) {//cam
		line(img1,
			Point(A1, A2),
			Point(R1, R2),
			Scalar(255, 255, 100, 99), 1);
		line(img1,
			Point(B1, B2),
			Point(D1, D2),
			Scalar(255, 255, 100, 99), 1);
		line(img1,
			Point(A1, A2),
			Point(B1, B2),
			Scalar(255, 255, 100, 99), 1);
		line(img1,
			Point(R1, R2),
			Point(D1, D2),
			Scalar(255, 255, 100, 99), 1);
		for (int i = 1; i < 20; i++) {
			line(img1,
				Point(248.5, -1144),
				Point(-100.5+i*33.7218, 880),
				Scalar(100, 100, 100, 50), 1);
			line(img1,
				Point(0, A2 + ConsHp * tan(asin(Lmaxp*i * sinB / pow(ConsHp * ConsHp + Lmaxp * i * Lmaxp * i - Lmaxp * i * H2cosBp, 0.5)))),
				Point(480, A2 + ConsHp * tan(asin(Lmaxp*i * sinB / pow(ConsHp * ConsHp + Lmaxp * i * Lmaxp * i - Lmaxp * i * H2cosBp, 0.5)))),
				Scalar(100, 100, 100, 50), 1);
		}
		for (int i = 1; i < 20; i++) {
			line(img0,
				Point(24*i, 0),
				Point(24*i, 848),
				Scalar(100, 100, 100, 50), 1);
			line(img0,
				Point(0, 42.4 * i),
				Point(480, 42.4 * i),
				Scalar(100, 100, 100, 50), 1);
		}
		
	}

	void labels(const Mat& cam_img, const Mat& img_mask, Mat& spectrum, int& posX, int& posY, double Area) {
		Mat img_labels,
			stats,
			centroids;
		int numoflables,
			idx = 0;			//라벨 번호

		Area = -1;
		posX = -1,
		posY = -1; // 찾은 라벨이 없을 경우


		if (!(numoflables
			= connectedComponentsWithStats(img_mask, img_labels, stats, centroids, 8, CV_32S))) {

			spectrum -= 1;
		}
		else { //작은 라벨 무시, 큰 영역의 라벨 찾기
			for (int j = 1; j < numoflables; j++) {
				int area = stats.at<int>(j, CC_STAT_AREA);

				if ((area > MINAREA) & (area > Area)) {
					//cout << area << endl;
					Area = area;
					idx = j;
				}
			}// for End

			//라벨 그리기
			for (int j = 1; j < numoflables; j++) {
				int lab_left = stats.at<int>(j, CC_STAT_LEFT);
				int lab_top 
					= stats.at<int>(j, CC_STAT_TOP);
				int lab_width = stats.at<int>(j, CC_STAT_WIDTH);
				int lab_heigh = stats.at<int>(j, CC_STAT_HEIGHT);

				circle(img_mask,
					Point(lab_left + (lab_width / 2), lab_top + (lab_heigh / 2)),
					lab_width + lab_heigh,
					255,
					3);

				if (j == idx) {
					//사용할 라벨 그리기
					posX = lab_left + (lab_width / 2);
					posY = lab_top + (lab_heigh / 2);

					//cam_img
					rectangle(cam_img,
						Point(lab_left, lab_top),
						Point(lab_left + lab_width, lab_top + lab_heigh),
						Scalar(255, 0, 255), 1);
					rectangle(cam_img,
						Point(posX, posY),
						Point(posX, posY),
						Scalar(255, 0, 255, 100), 1);
				}
				else {
					//사용하지 않는 라벨 그리기
					rectangle(cam_img,
						Point(lab_left, lab_top),
						Point(lab_left + lab_width, lab_top + lab_heigh),
						Scalar(0, 0, 175, 100), 1);

					/*rectangle(img_mask1,				//공 가리기
						Point(lab_left, lab_top),
						Point(lab_left + lab_width, lab_top + lab_heigh),
						Scalar(0, 0, 170), -1);*/
				}
			}// for End

			spectrum = (spectrum - 5) | img_mask;
		}
	}//labels End
};
#endif