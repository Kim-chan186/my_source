#pragma once
#ifndef SAVE_CELL_H
#define SAVE_CELL_H


#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

#include <map>
#include <opencv2/core/types.hpp>

//ÅÛÇÃ¸´À¸·Î ¸¸µé¾î¾ß µÈ´Âµ¥ ±ÍÂú´Ù...
using namespace cv;

namespace std {
	
	ofstream& operator << (ofstream& ofs, Point3d p3d) {
		ofs << (int)p3d.x << ", " << (int)p3d.y << ", " << p3d.z;
		return ofs;
	}

	Mat& operator << (Mat& mat, Point3d p3d) {

		if (p3d.z > 199) {
			cout << "save_cell.h error : over the depth 255 !! \n";
		}
		else{
			Mat mat_buf = Mat(48, 84, CV_8UC1, Scalar(0));
			rectangle(mat_buf,
				Point((int)p3d.y / 10, (int)p3d.x / 10),
				Point((int)p3d.y / 10, (int)p3d.x / 10),
				Scalar(p3d.z), 1);
			//mat.at<Vec3b>((int)p3d.y, (int)p3d.x)[0] = p3d.z;
			mat |= mat_buf;
		}
		return mat;
	}
	bool operator == (Point3d p3d_1, int n) {

		return ((int)p3d_1.x == n) & ((int)p3d_1.y == n) & ((int)p3d_1.z == n);
	}
	bool operator != (Point3d p3d_1, int n) {

		return ((int)p3d_1.x != n) | ((int)p3d_1.y != n) | ((int)p3d_1.z != n);
	}

	struct strNode_p3d {
		Point3d p3d = Point3d(0,0,0);						//0,0,0
		strNode_p3d* next = nullptr;		//nullptr
	};

	class node_p3d {			//FIFO
		strNode_p3d* _Node = nullptr;
		strNode_p3d* _Head = nullptr;

	public :
		node_p3d() {
			this->_Node = new strNode_p3d;
			this->_Head = this->_Node;
		}
		node_p3d(Point3d p3d) : node_p3d() {
			this->push(p3d);
		}

		void push(Point3d p3d) {
			if (p3d == 0) {
				cout << "  save_cell.h error : Point3d is 0_token!!" << endl;
			}
			else if(this->_Node->p3d == 0){
				this->_Node->p3d = p3d;
			}
			else {
				this->_Node = this->_Node->next = new strNode_p3d;
				this->_Node->p3d = p3d;
			}
		}
		Point3d pop() {

			Point3d null_p3d(0,0,0);
			strNode_p3d*	buffer_Np  = this->_Head;
			if (buffer_Np == nullptr) {
				return null_p3d;
			}
			Point3d			buffer_p3d = this->_Head->p3d;
			if (buffer_p3d == 0) {
				//cout << "  save_cell.h error : not enough Point3d !!" << endl;
				return null_p3d;
			}

			if (buffer_Np->next == nullptr) {
				this->_Head->p3d = null_p3d;
			}
			else {
				this->_Head = this->_Head->next;
				delete buffer_Np;
				buffer_Np = nullptr;
			}

			return buffer_p3d;
			
		}
		strNode_p3d*& getHead() {
			return this->_Head;
		}
		~node_p3d(){
		}
	};//Node_p3d


	class cell {
	
		const string file = "point3d.csv";
		const string file2 = "graph.csv";


		void error(const int n) const {
			switch (n) {
			case 1 :
				cout << "save_cell.h error : file isn't open !!";
				break;
			case 2:
				cout << "save_cell.h error : not enough Point_3_double !!";
				break;
			default :
				cout << "save_cell.h error : Undefined type error";
				break;
			}
		}
	public:
		void print_p3d(node_p3d Np3d, Mat& mat) {
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
					Point(i*50, 0 * i),
					Point(i*50, 480 * i),
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

			Point3d p3d1(400, 200, 400),
					p3d2 = Np3d.pop();
			bool flag1 = 1;
			bool flag2 = 1;

			for (int i=20; (p3d1 = Np3d.pop()) != 0; i+=20) {
				save_3d(p3d1.x, p3d1.y, p3d1.z);
				line(mat,
					Point(i+ 20,	(int)(450 - p3d1.x*2)),
					Point(i,		(int)(450 - p3d2.x*2)),
					Scalar(200, 40, 40),
					1);
				line(mat,
					Point(i + 20,	(int)(200 - p3d1.y*2)),
					Point(i,		(int)(200 - p3d2.y*2)),
					Scalar(40, 200, 40),
					1);
				line(mat,
					Point(i + 20,	(int)(450 - p3d1.z*4)),
					Point(i,		(int)(450 - p3d2.z*4)),
					Scalar(40, 40, 200),
					1);

				if (p3d1.x < 100 & flag1) {
					flag1 = 0;
					line(mat,
						Point(i + 20, 0),
						Point(i + 20, 480),
						Scalar(193, 50, 133),
						1);
				}
				else if (p3d1.x < 60 & flag2) {
					flag2 = 0;
					line(mat,
						Point(i + 20, 0),
						Point(i + 20, 480),
						Scalar(200, 70, 150),
						1);
				}

				p3d2 = p3d1;
			}//for End

			putText(mat, "X", Point(5, 475), FONT_HERSHEY_PLAIN, 1.2, Scalar(200, 40, 40));
			putText(mat, "Y", Point(25, 475), FONT_HERSHEY_PLAIN, 1.2, Scalar(40, 200, 40));
			putText(mat, "Z", Point(45, 475), FONT_HERSHEY_PLAIN, 1.2, Scalar(40, 40, 200));

		}//print_p3d End


		void save_3d(const Point3d p3d) const {
			ofstream cell(file.c_str(), ios::app);
			if (cell.is_open()) {
				cell << p3d << endl;
			}
			else {
				this->error(1);
			}
			cell.close();
			return;
		}
		void save_3d(const double x, const double y, const double z) const{
			ofstream cell(file.c_str(), ios::app);
			if (cell.is_open()) {
				cell << x << ", " << y << ", " << z << endl;
			}
			else {
				this->error(1);
			}
			cell.close();
			return;
		}
		void save_3d(node_p3d Np3d, unsigned int num = 0) { // num == 0 is all
			cout << "call save cell !! \n";
			if (num == 0) {
				Point3d p3d;
				ofstream cell(file.c_str(), ios::out);
				if (cell.is_open()) {
					for (; !((p3d = Np3d.pop()) == 0);) {
						cell << p3d.x << ", " << p3d.y << ", " << p3d.z << endl;
					}
				}
				else {
					this->error(1);
				}
				cell.close();
			}
			else
			{
				Point3d p3d;
				ofstream cell(file.c_str(), ios::out);
				if (cell.is_open()) {
					for (int i = 0; i<num; i++) {
						p3d = Np3d.pop();
						if (p3d == 0) {
							this->error(2);
							cell.close();
							return;
						}
						cell << p3d.x << ", " << p3d.y << ", " << p3d.z << endl;
					}
				}
				else {
					this->error(1);
				}
				cell.close();
			}//if end
			return;
		}//save_3d end
		void save_graph(node_p3d Np3d, Mat& mat) {
			mat = Mat(48, 84, CV_8UC1, Scalar(0));//480, 848
			Mat resize_mat;
			Point3d p3d;
			
			ofstream cell(file2.c_str(), ios::out);
			if (cell.is_open()) {
				for (;;) {
					if ((p3d = Np3d.pop()) == 0) {
						break;
					}
					mat << p3d;
				}
				
				resize(mat, resize_mat, Size(480, 848), 0, 0, 1);
		
				for (int i = 0; i < 48; i++) {
					mat.at<Vec3b>(i, 0)[0] = i + 1;
				}		
				cell << mat;
				mat = resize_mat;
			}
			else {
				this->error(1);
			}
			cell.close();
		}
	};//cell end

	
}
#endif
