#pragma once
#ifndef TRANSF_MAP_H
#define TRANSF_MAP_H


#include <iostream>
#include <iomanip>
#include <string>
#include <cstring>
#include <fstream>
#include <cmath>
#include <map>
#include <vector>
#include <omp.h>
#include <thread>
#include <chrono>
#include "CLinear_actu.h"

#include <opencv2/core/types.hpp>
#include "Communication.h"
#include "regression.h"
#include "mat_help.h"

#define GX			.33
#define GY			.30
#define GZ			10
#define Z_CRR		10 ///     9.8 / 0.16 
#define H			190
#define X_PIXEL		424 ///	    848 / 2
#define Y_PIXEL		240 ///     480 / 2
#define MS			.0166666
#define ELASTICITY	.84
#define ELASTICITY2	1.01

#define ALPHA		0.84

#define PPP			printf(

namespace std {

	struct rPoint // 이거 지워야 하는데 귀찮음
	{
		double x = 0;
		double y = 0;
		double z = 0;
	};


	cv::Point3d rp2p3d(cv::Point3d rp, cv::Point3d rp0) {
		Point3d p3d, p3d0;
		p3d.y = Lmax * (R2 - rp.y) / (D2 - B2);
		double  x1 = (rp.y - A2)*((A1 - R1) / (A2 - R2)) + A1,
			x2 = (rp.y - D2)*((B1 - D1) / (B2 - D2)) + D1;
		p3d.y = ConsH * tan(asin(p3d.y * sinB / pow(ConsH * ConsH + p3d.y * p3d.y - p3d.y * H2cosB, 0.5))) + ConsIN;
		p3d.x = 152.5 * (rp.x - x1) / (x2 - x1) - 76.25;
		p3d0.x = CWS + CWL * rp0.x / 480 - 76.25;
		p3d0.y = CHS + CHL * (848 - rp0.y) / 848;
		//cout << p3d << "    " << p3d0 << endl;
		p3d.z = (p3d0.y - p3d.y) / ((ConsIN - p3d.y) / ConsH - (ConsIN0 - p3d0.y) / ConsH0) - ConsZ;
		p3d.y = (ConsIN - p3d.y) * p3d.z / ConsH + p3d.y;
		p3d.x = p3d.x * (ConsH - p3d.z) / ConsH;
		return p3d;
	}
	ofstream& operator << (ofstream& ofs, rPoint point) {
		ofs << point.x << " " << point.y;
		return ofs;
	}
	ostream& operator << (ostream& ofs, rPoint point) {
		ofs << "(" << setw(3) << point.x << ", " << setw(3) << point.y << ")";
		return ofs;
	}


	class vect {
		int *_x, *_y, *_z, cont, flag,
			C_flag;
		//vector < vector<double>>	vecvec;
		//vector<double>				vx1, vx2, vx3,
		//							vy1, vy2, vy3,
		//							vz1, vz2, vz3;
		vector<double>				vx, vy, vz;
		Mat				mat;
		CLinear_actu C;
	public:
		double CalculateBounce(double a_factor, double b_factor, double c_factor, double t) {
			/*PPP "%.3f, %.3f, %.3f \n", a_factor, b_factor, c_factor);
			double vy, time;
			a_factor;
			b_factor;
			c_factor = (c_factor - 2); //z=a*t*t+b*t+c;  //bounce at 0.02m

			time = (-sqrt(b_factor*b_factor - 4 * a_factor*c_factor) - b_factor) / (a_factor);//0.2954;
			vy = 0.9 * sqrt(2 * 981 * (b_factor * b_factor / (4 * (-a_factor)) + c_factor));
			//0.9*981*(-sqrt(b_factor*b_factor - 4 * a_factor*c_factor) / (2 * a_factor));

			return (-3.779*(t - time) + (3.779 / 981)*(vy + 3.779)*(1 - exp(-(981) / (3.779)*(t - time)))) + 2;
			//(-3.779*(t - time) + (3.779/981)*(vy + 3.779)*(1 - exp(-(981)/(3.779)*(t - time)))) + 2; //z_factor*/

			double _time = (sqrt(b_factor*b_factor - 4 * a_factor*(c_factor-2)) + b_factor) / (-a_factor*2);
			if (_time < t) {
				t -= sqrt(b_factor*b_factor - 4 * a_factor*c_factor) / (-a_factor);
				return ((a_factor*t*t + b_factor *t + c_factor) - 2) * ELASTICITY;
			}
			else {
				return (a_factor*t*t + b_factor*t + c_factor);
			}
			

		}
		void Lmove0() {
			if (this->C_flag == 30) {
				this->C.start_move_actu(_Mxp);	// _Y
				this->C_flag = 0;
			}
			else {
				this->C_flag++;
			}
		}
		void CalculateLine(double &a_factor, double &b_factor, double crosspoint) {
			b_factor = (1 - ALPHA)*a_factor * crosspoint + b_factor;
			a_factor *= ALPHA;
		}
		//CalculateBounce End
		void show_mat() {
			putText(mat, "X", Point(5, 475), FONT_HERSHEY_PLAIN, 1.2, Scalar(200, 40, 40));
			putText(mat, "Y", Point(25, 475), FONT_HERSHEY_PLAIN, 1.2, Scalar(40, 200, 40));
			putText(mat, "Z", Point(45, 475), FONT_HERSHEY_PLAIN, 1.2, Scalar(40, 40, 200));

			cv::imshow("m", mat);
		}
		int get_flag() {
			return this->flag;
		}

		vect(int n = 200) {
			this->_x = new int[n];
			this->_y = new int[n];
			this->_z = new int[n];

			this->cont = 0,
			this->flag = 1;
			this->C_flag = 0;

			mat = Mat(480, 848, CV_8UC3, Scalar::all(30));

			for (int i = 1; i < 10; i++) {
				line(mat,
					Point(0, 50 * i),
					Point(848, 50 * i),
					Scalar(70, 70, 70),
					1);
			}
			for (int i = 1; i < 3; i++) {
				line(mat,
					Point(0, 200 * i + 50),
					Point(848, 200 * i + 50),
					Scalar(200, 200, 200),
					1);
			}
			for (int i = 1; i < 17; i++) {
				line(mat,
					Point(i * 50, 0 * i),
					Point(i * 50, 480 * i),
					Scalar(70, 70, 70),
					1);
			}
			for (int i = 1; i < 5; i++) {
				line(mat,
					Point(i * 200, 0 * i),
					Point(i * 200, 480 * i),
					Scalar(200, 200, 200),
					1);
			}
		}

		void vect_clear(int n = 200) {
			this->cont = 0,
			this->flag = 1;
			this->C_flag = 0;
			printf("clear\n");

			delete[] this->_x,
				this->_y,
				this->_z;

			this->_x = new int[n];
			this->_y = new int[n];
			this->_z = new int[n];

			//this->vx.clear();
			//this->vy1.clear();
			//this->vy2.clear();
			//this->vz1.clear();
			//this->vz2.clear();
			this->vx.clear();
			this->vy.clear();
			this->vz.clear();


			mat = Mat(480, 848, CV_8UC3, Scalar::all(30));

			for (int i = 1; i < 10; i++) {
				line(mat,
					Point(0, 50 * i),
					Point(848, 50 * i),
					Scalar(70, 70, 70),
					1);
			}
			for (int i = 1; i < 3; i++) {
				line(mat,
					Point(0, 200 * i + 50),
					Point(848, 200 * i + 50),
					Scalar(200, 200, 200),
					1);
			}
			for (int i = 1; i < 17; i++) {
				line(mat,
					Point(i * 50, 0 * i),
					Point(i * 50, 480 * i),
					Scalar(70, 70, 70),
					1);
			}
			for (int i = 1; i < 5; i++) {
				line(mat,
					Point(i * 200, 0 * i),
					Point(i * 200, 480 * i),
					Scalar(200, 200, 200),
					1);
			}
		}

		void push(cv::Point3d& p3d, cv::Point3d& p3d0) {
			if (p3d.x == -1) {
				PPP "왼 카메라 tracking error \n");
			}
			else if (p3d0.x == -1) {
				PPP "오른카메라 tracking error \n");
			}
			else {
				p3d = rp2p3d(p3d, p3d0);

				if ((cont == 200)) { //| (p3d0.y > 820 & this->flag != 1)
					this->vect_clear();
				}
				else if (this->flag == 1) {
					if ((p3d0.y < 230)) {
						this->flag = 2;
					}
					return; //값 생략
				}
				else if (this->flag == 2) {
					cout << cont << "  " << p3d << endl;
					*(this->_x + this->cont) = p3d.x;
					*(this->_y + this->cont) = p3d.y;
					*(this->_z + this->cont++) = p3d.z;

					if ((this->flag == 2) & (p3d.y < 100)) {
						double x, t, z;
						this->make_vect2(x, t, z);
						this->flag = 0;

						printf("%.4fcm     %.4fs     %.4fcm \n", x, t, z);

						return;
					}
				}
			}
		}//push end

		void make_vect2(double& x, double& t, double& z) {
			//PPP "start make vec\n");
			int bound_point = this->cont - 1;
			double y;
			vector<int>				 vpoint;
			vector<double>			 vdepth;
			vector<int>::iterator	 itp;
			vector<double>::iterator itd;
			for (int i = 0; i < this->cont; i++) {
				if (*(this->_z + i) < 10) {
					vpoint.emplace_back(i);
				}
			}
			for (auto& it : vpoint) {
				vdepth.emplace_back(*(this->_z + it));
			}
			sort(vdepth.begin(), vdepth.end());

			/*while (vdepth.size() > 2) {
				itd = vdepth.end();
				for (auto& it : vpoint) {
					if (*(this->_z + it) == *itd) {
						it = 999;

						vdepth.pop_back();
						break;
					}
				}
			}
			for (itp = vpoint.begin(); !(itp == vpoint.end()); itp++) {
				if (*itp == 999) {
					itp = vpoint.erase(itp);
					itp--;
				}
			}*/


			int vp_flag = 1,		//vpoint.size(),
				_start, _end;
			for (auto& it : vpoint) {
				if (*(this->_z + it) == vdepth.at(0)) {
					bound_point = it;
					break;
				}
			}
			if (vp_flag == 2) {
				_start = vpoint.at(0), _end = vpoint.at(1);
				//printf("vp_flag : 2,   p1 : %d,   p2 :  %d \n", vpoint.at(0), vpoint.at(1));
			}
			else if (vp_flag == 1) {
				if (bound_point > cont / 2)
					_start = 0, _end = bound_point;
				else
					_start = bound_point, _end = this->cont;

				//printf("vp_flag : 1,   p1 : %d \n", bound_point);
			}
			else {
				printf("잘못 만듬");
			}
			double  ax = 0, bx = 0, ay = 0, by = 0, az = 0, bz = 0, cz = 0,
				ax2 = 0, bx2 = 0, ay2 = 0, by2 = 0, az2 = 0, bz2 = 0, cz2 = 0;


#pragma omp parallel 
			{
#pragma omp sections
				{
#pragma omp section
					{
						for (int i = _start; i <= _end; i++) {
							vx.emplace_back(*(this->_x + i));
						}
					}
#pragma omp section
					{
						for (int i = _start; i <= _end; i++) {
							vy.emplace_back(*(this->_y + i));
						}
					}
#pragma omp section
					{
						for (int i = _start; i <= _end; i++) {
							vz.emplace_back(*(this->_z + i));
						}
					}
				}

#pragma omp sections
				{
#pragma omp section
					{
						linearRegression2(this->vx.size(), this->vx, ax, bx, _start);
						if (isnan(ax) != 0) {
							printf("az1 is nan \n");
						}
						ax2 = ax;
						bx2 = bx;

						//CalculateLine(ax2, bx2, _end);
					}//section end
#pragma omp section
					{
						linearRegression2(this->vy.size(), this->vy, ay, by, _start);
						if (isnan(ay) != 0) {
							printf("ay is nan \n");
						}
						ay2 = ay;
						by2 = by;

						CalculateLine(ay2, by2, _end);
					}//section end
#pragma omp section
					{
						polyRegression(this->vz.size(), this->vz, az, bz, cz, _start);
						if (isnan(az) != 0) {
							printf("az1 is nan \n");
						}
					}
				}//section end
			}//parallel end
			cout << az << "     " << bz << "     " << cz << endl;
			y = (by2+16) / -ay2;
			x = ax2 * y + bx2;
			z = this->CalculateBounce(az, bz, cz, y);
			//z = this->CalculateBounce(az, bz, cz, y);//(az * (2 * _end - y)* (2 * _end - y) + bz * (2 * _end - y) + cz)*0.80;

			t = (y - cont - 1)*MS;

			udpClient(t, x, z);
			this->C.start_move_actu(x);
			//C.move_actu(x);
			//position = x;



			line(this->mat, Point(_end * 20 + 20, 0),	Point(_end * 20 + 20, 480), Scalar(200, 100, 130, 70), 2);
			line(this->mat, Point(y * 20 + 20, 0),		Point(y * 20 + 20, 480),	Scalar(200, 100, 130, 70), 2);

			vector<double> p3d_vec;
			for (int i = 0; i < cont; i++) {
				p3d_vec.emplace_back(*(_x + i));
				p3d_vec.emplace_back(*(_y + i));
				p3d_vec.emplace_back(*(_z + i));
			}
			this->print_p3d(p3d_vec, .5);
			p3d_vec.clear();
			this->show_mat();
			
			
			for (int i = 0; (ay * i + by) > 0; i++) {

				if (i < _start) {
					p3d_vec.emplace_back(-200);
					p3d_vec.emplace_back(-200);
					p3d_vec.emplace_back(-200);
				}
				else if (i == _start) {
					p3d_vec.emplace_back(-200);
					p3d_vec.emplace_back(-200);
					p3d_vec.emplace_back(-200);

					p3d_vec.emplace_back(-999);

					p3d_vec.emplace_back(ax * i + bx);
					p3d_vec.emplace_back(ay * i + by);
					p3d_vec.emplace_back(az * i* i + bz * i + cz);
				}
				else if (i > _end) {
					p3d_vec.emplace_back(ax2 * i + bx2);
					p3d_vec.emplace_back(ay2 * i + by2);
					p3d_vec.emplace_back(this->CalculateBounce(az, bz, cz, i));
				}
				else if (i < _end) {
					p3d_vec.emplace_back(ax * i + bx);
					p3d_vec.emplace_back(ay * i + by);
					p3d_vec.emplace_back(az * i* i + bz * i + cz);
				}
				else { // ==
					p3d_vec.emplace_back(ax * i + bx);
					p3d_vec.emplace_back(ay * i + by);
					p3d_vec.emplace_back(az * i* i + bz * i + cz);

					p3d_vec.emplace_back(-999);

					p3d_vec.emplace_back(ax2 * i + bx2);
					p3d_vec.emplace_back(ay2 * i + by2);
					p3d_vec.emplace_back(this->CalculateBounce(az, bz, cz, i));

				}
			}

			this->print_p3d(p3d_vec, 1.2);
			this->show_mat();

			this->vect_clear();
		}

		void print_p3d(vector<double>& p3d_vec, double color = 1) {

			vector<double>::iterator it = p3d_vec.begin();
			int vec_count = p3d_vec.size() / 3;
			bool flag1 = 1;
			//bool flag2 = 1;

			double	x1;
			double  x2 = *it++;
			double	y1,
				y2 = *it++;
			double	z1,
				z2 = *it++;

			for (int i = 1; i < vec_count; i++) {
				x1 = *it++;
				if (x1 == -999) {
					vec_count--;

					x1 = *it++,
						y1 = *it++,
						z1 = *it++;

					line(mat,
						Point(i * 20, (int)(200 - x1 * 2)),
						Point(i * 20, (int)(200 - x2 * 2)),
						Scalar(200 * color, 40 * color, 40 * color),
						2);
					line(mat,
						Point(i * 20, (int)(450 - y1 * 2)),
						Point(i * 20, (int)(450 - y2 * 2)),
						Scalar(40 * color, 200 * color, 40 * color),
						2);
					line(mat,
						Point(i * 20, (int)(450 - z1 * 5)),
						Point(i * 20, (int)(450 - z2 * 5)),
						Scalar(40 * color, 40 * color, 200 * color),
						2);
					x2 = x1,
						y2 = y1,
						z2 = z1;
					x1 = *it++;
				}
				y1 = *it++,
					z1 = *it++;
				line(mat,
					Point(i * 20 + 20, (int)(200 - x1 * 2)),
					Point(i * 20, (int)(200 - x2 * 2)),
					Scalar(200 * color, 40 * color, 40 * color),
					2);
				line(mat,
					Point(i * 20 + 20, (int)(450 - y1 * 2)),
					Point(i * 20, (int)(450 - y2 * 2)),
					Scalar(40 * color, 200 * color, 40 * color),
					2);
				line(mat,
					Point(i * 20 + 20, (int)(450 - z1 * 5)),
					Point(i * 20, (int)(450 - z2 * 5)),
					Scalar(40 * color, 40 * color, 200 * color),
					2);

				x2 = x1;
				y2 = y1;
				z2 = z1;

			}//for End
		}
		~vect() {

			delete[] this->_x,
				this->_y,
				this->_z;
		}
	};

	class file {
		const string str = "map.txt";

	protected:
		void error(const int n) const {
			switch (n) {
			case 1:
				cout << "Transf_map.h error : file isn't open !!" << endl;
				break;
			case 2:
				cout << "Transf_map.h error : load() is Undefined num == 0 " << endl;
				break;
			case 3:
				cout << "Transf_map.h error : map is can control 10 point " << endl;
				break;
			default:
				cout << "Transf_map.h error : Undefined type error" << endl;
				break;
			}
		}

	public:
		file() {};
		void clear() {
			ofstream txt(str.c_str(), ios::out);
			if (txt.is_open()) {

			}
			else {
				this->error(1);
			}
			txt.close();
			return;
		}
		void save(const rPoint point) const {
			ofstream txt(str.c_str(), ios::app);
			if (txt.is_open()) {
				txt << point << endl;
			}
			else {
				this->error(1);
			}
			txt.close();
			return;
		}
		void save(const double x, const double y) const {
			ofstream txt(str.c_str(), ios::app);
			if (txt.is_open()) {
				txt << x << " " << y << endl;
				//cout << x << ", " << y << ", " << z << endl;
			}
			else {
				this->error(1);
			}
			txt.close();
			return;
		}
		void save_all(const rPoint* ppoint, const unsigned int count) {
			ofstream txt(str.c_str(), ios::out);
			if (txt.is_open()) {
				for (int i = 0; i < count; i++)
					txt << *(ppoint + i) << endl;
			}
			else {
				this->error(1);
			}
			txt.close();
			cout << "save all about \"" << str << "\"" << endl;
			return;
		}
		rPoint& load(const unsigned int num = 0) const {//몰라 너무 큰 번호 부르지마 0나오니까
			rPoint point;
			if (num == 0) {//all
				this->error(2);
				return point;
			}
			ifstream txt(str.c_str(), ios::app);
			if (txt.is_open()) {
				string str;
				for (int i = 0; i < num; i++)
					getline(txt, str);
				char  c_str[35];
				strcpy_s(c_str, 35, str.c_str());
				int i = 0,
					j = 0;
				char buf1[35],
					buf2[35];

				while (*(c_str + i) != ' ') {
					buf1[i++] = *(c_str + i);
				}
				point.x = atoi(buf1);

				while (*(c_str + i) != NULL) {
					buf2[j++] = *(c_str + i++);
				}
				point.y = atoi(buf2);

			}
			else {
				this->error(1);
			}
			txt.close();

			return point;
		}//lode end;
		rPoint*& load_all(int& count) {	//max = 10
			rPoint* ppoint = new rPoint[10];
			rPoint* ppoint_buf;
			ifstream txt(str.c_str(), ios::app);

			if (txt.is_open()) {
				char c_str[35];
				for (count = 0; txt.getline(c_str, 35); count++) {
					if (*c_str == ' ') {
						break;
					}
					int i = 0,
						j = 0;
					char	buf1[35],
						buf2[35];

					while (*(c_str + i) != ' ') {
						buf1[i++] = *(c_str + i);
					}
					ppoint[count].x = atoi(buf1);

					while (*(c_str + i) != NULL) {
						buf2[j++] = *(c_str + i++);
					}
					ppoint[count].y = atoi(buf2);
				}

				rPoint* ppoint_buf = new rPoint[count];
				for (int i = 0; i < count; i++) {
					*(ppoint_buf + i) = *(ppoint + i);
				}
				delete[] ppoint;
				txt.close();
				return ppoint_buf;
			}
			else {
				this->error(1);
			}
			count = 0;
			return ppoint;
		}//lode end;


	};//file end

	class rmap {
		int count = 0;					// 0 : 초기상태,	 -1 : delete
		rPoint* ppoint;
		file File;

		void error(const int n) const {
			switch (n) {
			case 1:
				cout << "Transf_map.h error : rmap is deleted !!" << endl;
				break;
			case 2:
				cout << "Transf_map.h error : rmap isn't already opened!!" << endl << "  file is opening now" << endl;
				break;
			case 3:
				cout << "Transf_map.h error : rmap is Undefined when num == 0" << endl;
				break;
			case 4:
				cout << "Transf_map.h error : plz calling add, not change" << endl;
				break;
			case 5:
				cout << "Transf_map.h error : input num is not correct, changing number the '" << count + 1 << "'" << endl;
				break;
			default:
				cout << "Transf_map.h error : Undefined type error" << endl;
				break;
			}
		}

	public:
		rmap() {
			this->load_map();
		}
		rmap(const unsigned int px, const unsigned int py = 0, const unsigned int num = 1) {
			this->load_map();

			rPoint point;
			point.x = px;
			point.y = py;
			if (num == 0) {
				error(3);
				return;
			}
			else if (num <= count) {
				this->change(point, num);
			}
			else {
				if (num != count + 1)
					error(5);
				add(point);
			}
		}

		rmap(rPoint point, const unsigned int num = 1) {
			this->load_map();

			if (num == 0) {
				error(3);
				return;
			}
			else if (num <= count) {
				this->change(point, num);
			}
			else {
				if (num != count + 1)
					error(5);
				add(point);
			}
		}
		rmap(rPoint* ppoint, const unsigned int count = 1) {
			this->load_map();

			change_all(ppoint, count);
		}

		void add(rPoint point) {
			if (count == -1) {
				count++;
			}
			rPoint* ppoint_buf;
			ppoint_buf = ppoint;
			ppoint = new rPoint[++count];
			for (int i = 0; i < count; i++) {
				*(ppoint + i) = *(ppoint_buf + i);
			}
			*(ppoint + count - 1) = point;

		}
		void change(const rPoint point, const unsigned int num) {
			if (num == 0) {
				error(3);
				return;
			}
			else if (num <= count) {
				*(this->ppoint + num - 1) = point;
			}
			else {
				error(4);
			}
		}
		void change_all(rPoint* ppoint, const unsigned int count) {
			this->count = count;
			this->ppoint = ppoint;
		}
		void del() {
			count = -1;
			delete[] ppoint;
		}
		void load_map() {
			ppoint = File.load_all(count);
		}
		void print() {
			if (count == 0) {
				this->load_map();
				error(2);
			}
			else if (count == -1) {
				error(1);
				return;
			}

			for (int i = 0; i < count; i++) {
				cout << setw(2) << i + 1 << " : " << ppoint[i] << endl;
			}
		}

		double get_delta(rPoint p1, rPoint p2) {
			sqrt(pow((double)p1.x - p2.x, 2) / 10.89 + pow((double)p1.y - p2.y, 2) / 10.89 + pow(p1.z - p2.z, 2));
		}
		~rmap() {
			cout << "calling ~rmap" << endl;
			if ((count != 0) | (count != -1))
				File.save_all(this->ppoint, this->count);
			delete[] ppoint;
		}
	};
}

#endif

/*void make_vect2(double& t, double& y, double& z) {

			double standard = 2000;
			int standard_cont = -1,
				flag_bouns = 0;

			for (int i = 0; i < this->cont; i++) {
				if (standard > *(this->_z + i)) {
					standard_cont = i;
					standard = *(this->_z + i);
				}
			}
			double  ax = 0, ay1 = 0, ay2 = 0, az1 = 0, az2 = 0, cz1 = 0,
				bx = 0, by1 = 0, by2 = 0, bz1 = 0, bz2 = 0, cz2 = 0;

#pragma omp parallel
			{
#pragma omp sections
				{
#pragma omp section
					{
						for (int i = 0; i < this->cont; i++) {
							this->vx.emplace_back(*(this->_x + i));
						}
					}//section end
#pragma omp section
					{
						for (int i = 0; i <= standard_cont; i++) {
							this->vy1.emplace_back(*(this->_y + i));
						}
					}//section end
#pragma omp section
					{
						if (this->cont - 4 > standard_cont) {
							for (int i = standard_cont; i < this->cont; i++) {
								this->vy2.emplace_back(*(this->_y + i));
							}
						}
					}//section end
#pragma omp section
					{
						for (int i = 0; i <= standard_cont; i++) {
							this->vz1.emplace_back(*(this->_z + i));
						}
					}//section end
#pragma omp section
					{
						if (this->cont - 4 > standard_cont) {
							for (int i = standard_cont; i < this->cont; i++) {
								this->vz2.emplace_back(*(this->_z + i));
							}
						}
					}//section end

				}//sections end   //               // wait //               //

#pragma omp sections
				{
#pragma omp section
					{
						linearRegression2(this->vx.size(), this->vx, ax, bx);
						if (ax == 0) {
							printf("x 기울기 0 !! \n");
							ax = 1;
						}
						else if (ax > 0) {
							ax = -ax;
							printf("x 기울기 양수 !! \n");
						}

					}//section end
#pragma omp section
					{
						polyRegression(this->vz1.size(), this->vz1, az1, bz1, cz1);
						if (isnan(az2) != 0) {
							printf("az1 is nan \n");
						}
					}//section end
#pragma omp section
					{
						if (this->cont - 4 > standard_cont) {
							polyRegression(this->vz2.size(), this->vz2, az2, bz2, cz2, standard_cont);
							if (isnan(az2) != 0) {
								printf("az2 is nan \n");
							}
						}
					}//section end
#pragma omp section
					{
						linearRegression2(this->vy1.size(), this->vy1, ay1, by1);
					}//section end
#pragma omp section
					{
						if (this->cont - 1 > standard_cont) {
							linearRegression2(this->vy2.size(), this->vy2, ay2, by2, standard_cont);
						}
					}//section end


				}//sections end
			}//parallel end


			if (this->cont - 1 > standard_cont) {

				//printf("ay2 / ay1 = %f \n", ay2 / ay1);
			}
			else {
				ay2 = ay1;
				by2 = ay1;

				az2 = az1;
				bz2 = az1;
			}


			t = ((bx) / -ax - cont);
			double alpha = standard_cont + t;
			//cout << az1 << "   "<< bz1 << "   "<< cz1 <<endl;

			y = ay2 * (alpha + t) + by2;
			z = az2 * alpha * alpha
				+ bz2 * alpha
				+ cz2;
			t *= MS;

			vector<double> p3d_vec;
			for (int i = 0; i < cont; i++) {
				p3d_vec.emplace_back(*(_x + i));
				p3d_vec.emplace_back(*(_y + i));
				p3d_vec.emplace_back(*(_z + i));
			}
			this->print_p3d(p3d_vec, .5);


			p3d_vec.clear();

			for (int i = 0; i < this->cont; i++) {
				p3d_vec.emplace_back(ax * i + bx);

				if (i < standard_cont) {
					p3d_vec.emplace_back(ay1 * i + by1);
					p3d_vec.emplace_back(az1 * i* i + bz1 * i + cz1);
				}
				else if (i > standard_cont) {
					p3d_vec.emplace_back(ay2 * i + by2);
					p3d_vec.emplace_back(az2 * i* i + bz2 * i + cz2);
				}
				else { // ==
					p3d_vec.emplace_back(ay1 * i + by1);
					p3d_vec.emplace_back(az1 * i* i + bz1 * i + cz1);
					p3d_vec.emplace_back(-999);
					p3d_vec.emplace_back(ay2 * i + by2);
					p3d_vec.emplace_back(az2 * i* i + bz2 * i + cz2);
				}

			}
			this->print_p3d(p3d_vec, 1.2);
			line(this->mat, Point(standard_cont * 20 + 20, 0), Point(standard_cont * 20 + 20, 480), Scalar::all(0), 1);//Scalar(200, 100, 130, 99), 2);
			this->show_mat();

			this->vect_clear();
		}*/