#pragma once
#ifndef CLINEAR_ACTU
#define CLINEAR_ACTU

#include "AXL.h"
#include "AXM.h"
#include <iostream>
#include <stdlib.h>
#include <string.h>
using namespace std;

#pragma comment(lib, "ws2_32")	
#define PORT 10048
#define PACKET_SIZE 30
#define _Y 24
#define _Mxp 22
#define Weight 1

#define max_position 50
#define vel 3000
#define accel 1500
#define dccel 1500

DWORD uStatus;
double position;
double cmd_position;

//double now_pos;
//double pre_pos;
//bool direction_flag = true;
//bool pre_direction_flag = true;
//double dPos;
//bool linear_flag = false;

class CLinear_actu
{
	/*static double position;
	static double now_pos;
	static double pre_pos;
	static double new_pos;*/

	char mot_file[51] = "C://Users//admin//Desktop//Baram//ajin20190628.mot";
public:
	CLinear_actu()
	{
		DWORD Code = AxlOpen(7);   //시작 시 라이브러리 초기화 IRQ 번호는 초기에 7로 설정
		if (Code == AXT_RT_SUCCESS)
		{
			//printf("라이브러리가 초기화되었습니다.\n");
			//모션 모듈이 있는지 검사
			DWORD uStatus;
			Code = AxmInfoIsMotionModule(&uStatus);   // 현재 시스템에 장착되어있는 모션 모듈의 존재 여부 확인
			if (Code == AXT_RT_SUCCESS)   // 라이브러리 초기화가 성공했을 때
			{
				//printf("라이브러리가 초기화되었습니다.\n");
				if (uStatus == STATUS_EXIST)
				{
					printf("라이브러리가 초기화되었습니다.\n");

					AxmMotLoadParaAll(mot_file);   //mot 파일에 적혀있는 설정 정보들을 불러옴

					// 현재 위치를 원점으로 설정
					AxmStatusSetActPos(0, 0.0);
					AxmStatusSetCmdPos(0, 0.0);

					AxmSignalServoOn(0, ENABLE);// 서보 ON 채널 번호 0번 enable

					AxmMotSetAbsRelMode(0, 0); //0->abs, 1->Rel 채널번호 0, 위치 구동 상대모드 : 이동함수에서 사용하는 위치값은 현재위치에서 이동할 이동량이 된다.
					AxmMotSetProfileMode(0, 3);   //0->symetric trapezode, 1->unsymetric trapezode, 2->reserved, 3->symetric S Curve, 4->unsymetric S Cuve
				}
			}
		}
	}
	~CLinear_actu()
	{
		AxmSignalServoOn(0, 0);   // 서보 off
		AxlClose();   //메모리 반환
	}

	//void move_actu(double pos)
	//{
	//	if (pos <= 0)
	//		pos += _Y;
	//	else
	//		pos -= _Y;
	//	if (pos > max_position) {   // 리니어가 정해진 범위 안에 있을 때 수행 
	//		pos = max_position;
	//	}
	//	else if (pos < -max_position) {
	//		pos = -max_position;
	//	}
//
	//	if ((pos < max_position) & (pos > -max_position)) {   // 리니어가 정해진 범위 안에 있을 때 수행 
	//		AxmMovePos(0, pos, vel, accel, dccel);  // 요것은 기존에 쓰던 함수 펄스출력이 종료가 되어야 함수탈출 
	//	}
	//	//Sleep(200);
	//	//AxmMoveStartPos(0, 0, 1000, 500, 500);
	//}

	void start_move_actu(double pos) {

		//if (pos <= 0)
		//	pos += _Y;
		//else
		//	pos -= _Y;

		if (pos < -_Mxp) {
			pos = pos * Weight + _Y;
		}
		else if ((pos >= -_Mxp) & (pos <=0)) {
			pos += _Y;
		}
		else if ((pos > 0) & (pos < _Mxp)) {
			pos -= _Y;
		}
		else if (pos == _Mxp) {
			pos -= _Mxp;
		}
		else if ((pos > _Mxp)) {
			pos = pos * Weight - _Y;
		}




		if (pos > max_position) {   // 리니어가 정해진 범위 안에 있을 때 수행 
			pos = max_position;
		}
		else if (pos < -max_position) {
			pos = -max_position;
		}


		if ((pos <= max_position) & (pos >= -max_position)) {   // 리니어가 정해진 범위 안에 있을 때 수행 
			AxmMoveStartPos(0, pos, vel, accel, dccel);  // 요것은 기존에 쓰던 함수 펄스출력이 종료가 되어야 함수탈출  내 개인적인(황순혁) 생각으로는 밑에 지워도 이것땜시 안될듯
		}
		//AxmMovePos(0, 0, 1000, 500, 500);
	}





	double get_act_pose(){
		double dPos;
		AxmStatusGetActPos(0, &dPos);   // 채널 번호0, 위치
		return dPos;   //엔코더로 읽어들인 Actual 위치를 반환 한다.
	}

	//void override_move_actu(double pos) {
//	double p = this->get_act_pose();
//	if ((p + pos < max_position) & (p + pos > -max_position)) {
//		AxmOverridePos(0, pos);
//		cout << this->get_act_pose() << endl;
//		//AxmOverrideSetMaxVel(0, 1000);
//		//AxmOverrideVel(0, 300);
//	}
//}


	//static void testplay(double* pos) {
	//	while(true){
	//		printf("수행해라 ㅡㅡ 입력받은 포지션 : %f \n", position);
	//		
	//		while (now_pos == position); // 포즈값은 계속 들어온다 가장 마지막것으로 갱신해야함 계속 비교를 하면서 다를시 다음으로 넘어감
	//		//while (true) {
	//		//	unique_lock<mutex>lock(mut);
	//		//	while (!linear_flag)
	//		//		signal.wait(lock);
	//		//	linear_flag = false;
	//		now_pos = *pos;				// 새로운 포즈값을 현재 포즈로 대입
	//		printf("now_pos : %.1f\n", now_pos);
	//		printf("pre_pos : %.1f\n", pre_pos);
	//
	//		if ((pre_pos * now_pos) < 0) { // 이전 목표위치와 현재 목표위치의 방향이 반대일 때
	//			AxmMoveEStop(0); // 비상 정지
	//			printf("Emergency STOP\n");
	//			AxmMoveStartPos(0, *pos, vel, accel, dccel);
	//		}
	//		else {
	//			AxmStatusReadInMotion(0, &uStatus);
	//			if (uStatus) { // True 이면 구동중   
	//				AxmOverridePos(0, *pos);
	//				printf("OverridePOS : %.1f\n", now_pos);
	//			}
	//			else {
	//				AxmMoveStartPos(0, *pos, vel, accel, dccel);
	//				printf("MoveStart : %.1f\n", now_pos);
	//			}
	//		}
	//		pre_pos = now_pos;
	//	}
	//}

	/*static void linear_move(double y) {
		//while (true) {
		unique_lock<mutex>lock(mut);
		linear_flag = true;
		//Sleep(100);
		//srand(static_cast<int>(time(0)));

		//position = y; // rand() % 40; // * a;
		//if (booho) {
		//	a = -1;
		//	booho = false;
		//}
		//else {
		//	a = 1;
		//	booho = true;
		//}

		printf("입력받은 posiiton = %.1f\n", position);
		signal.notify_one();
		signal.wait(lock);
		lock.unlock();
		//}
	}*/

	//void play(double* pos) {
	//	while (true) {
	//		unique_lock<mutex>lock(mut);
	//		while (!flag)
	//			signal.wait(lock);
	//		flag = false;
	//		now_pos = *pos;
	//		if ((pre_pos * now_pos) < 0) { // 이전 목표위치와 현재 목표위치의 방향이 반대일 때
	//			AxmMoveEStop(0); // 비상 정지
	//			AxmMoveStartPos(0, *pos, vel, accel, dccel);
	//		}
	//		else {
	//			if (!(AxmStatusReadMotion(0, &uStatus))) { // 0 이면 구동중
	//				AxmOverridePos(0, *pos);
	//			}
	//			else {
	//				AxmMoveStartPos(0, *pos, vel, accel, dccel);
	//			}
	//		}
	//		pre_pos = now_pos;
	//		signal.notify_one();
	//		lock.unlock();
	//	}
	//}

};

/*
void linear_move(double check_y)
{
	while (true) {
		if (position > max_position) {   // 리니어가 정해진 범위 안에 있을 때 수행 
			position = max_position;
		}
		else if (position < -max_position) {
			position = -max_position;
		}
		AxmMovePos(0, position, vel, accel, dccel);  // 요것은 기존에 쓰던 함수 펄스출력이 종료가 되어야 함수탈출
		Sleep(3000);
		position = 0;
		AxmMovePos(0, 0, 56, 100, 100);
		//DWORD uStatus;
		//AxmStatusReadInMotion(0, &uStatus);
		//while (uStatus)
		//{
		//	AxmStatusReadInMotion(0, &uStatus);
		//}
		//AxmMovePos(0, 0, 50, 200, 200);
	}
}*/

//thread th(&CLinear_actu::testplay, &position);
//double CLinear_actu::position = 0;
//double CLinear_actu::now_pos = 0;
//double CLinear_actu::pre_pos = 0;
//double CLinear_actu::new_pos = 0;

/*
void testplay() {

	while (true) {
		//if (position == 1000) {
		//	AxmMovePos(0, 0, vel, accel, dccel);
		//}
		//else {
		do {
			//while (now_pos == pre_pos);
			Sleep(170);
			AxmStatusGetActPos(0, &dPos);
			AxmStatusGetCmdPos(0, &dPos);
			//if (position < max_position & position > -max_position) {
			//	if (position > 0)
			//		position -= _Y;
			//	else
			//		position += _Y;
			now_pos = position;
			//printf("now_pos : %.1f\n", now_pos);
			//printf("pre_pos : %.1f\n", pre_pos);
			//printf("real_pos: %.1f\n", dPos);
			//printf("now-pre : %.1f\n", now_pos - pre_pos);
			//cout << boolalpha << direction_flag << endl;
			//cout << boolalpha << pre_direction_flag << endl;
			//if (now_pos < pre_pos) {
			//	direction_flag = true;
			//}
			//else if (now_pos > pre_pos) {
			//	direction_flag = false;
			//}
			//else {
			//	direction_flag = pre_direction_flag;
			//}
			if (now_pos <= dPos) {
				direction_flag = true;
			}
			else {
				direction_flag = false;
			}

			if (direction_flag != pre_direction_flag) { // 이전 목표위치와 현재 목표위치의 방향이 반대일 때
				//AxmMoveSStop(0); // 비상 감속 정지
				AxmMoveEStop(0);
				AxmMoveStartPos(0, position, vel, accel, dccel);
				//printf("방향 바뀌어서 Emergency STOP\n\n");
			}
			else {
				AxmStatusReadInMotion(0, &uStatus);
				if (uStatus) { // True 이면 구동중   
					AxmOverridePos(0, position);
					//printf("OverridePOS : %.1f\n", now_pos);
				}
				else {
					AxmMoveStartPos(0, position, vel, accel, dccel);
					//printf("MoveStart : %.1f\n\n", now_pos);
				}
			}
			//pre_pos = now_pos;
			pre_direction_flag = direction_flag;
			//}
		} while (now_pos != dPos);// dPos);		//position != 1000
	}
	//}
}
*/

//void testplay() {
//
//	while (true) {
//		//do {
//			Sleep(170);
//			AxmStatusGetActPos(0, &dPos);
//			AxmStatusGetCmdPos(0, &dPos);
//			//if (position < max_position & position > -max_position) {
//			//	if (position > 0)
//			//		position -= _Y;
//			//	else
//			//		position += _Y;
//			now_pos = position;
//
//			if (now_pos <= dPos) {
//				direction_flag = true;
//			}
//			else {
//				direction_flag = false;
//			}
//
//			if (direction_flag != pre_direction_flag) { // 이전 목표위치와 현재 목표위치의 방향이 반대일 때
//				AxmMoveEStop(0);
//				AxmMoveStartPos(0, position, vel, accel, dccel);
//				//printf("방향 바뀌어서 Emergency STOP\n\n");
//			}
//			else {
//				AxmStatusReadInMotion(0, &uStatus);
//				if (uStatus) { // True 이면 구동중   
//					AxmOverridePos(0, position);
//					//printf("OverridePOS : %.1f\n", now_pos);
//				}
//				else {
//					AxmMoveStartPos(0, position, vel, accel, dccel);
//					//printf("MoveStart : %.1f\n\n", now_pos);
//				}
//			}
//			//pre_pos = now_pos;
//			pre_direction_flag = direction_flag;
//			//}
//		//} while (now_pos != dPos);
//	}
//}




void move_actu2()
{
	position = 25;
	while (true) {
		cmd_position = position;
		if (cmd_position > max_position) {   // 리니어가 정해진 범위 안에 있을 때 수행 
			cmd_position = max_position;
		}
		else if (cmd_position < -max_position) {
			cmd_position = -max_position;
		}

		if (cmd_position <= 0)		cmd_position += 25;
		else						cmd_position -= 25;

		if ((cmd_position < max_position) & (cmd_position > -max_position)) {   // 리니어가 정해진 범위 안에 있을 때 수행 
			AxmMovePos(0, cmd_position, vel, accel, dccel);  // 요것은 기존에 쓰던 함수 펄스출력이 종료가 되어야 함수탈출  내 개인적인(황순혁) 생각으로는 밑에 지워도 이것땜시 안될듯
		}
		Sleep(18);
		position = 25;
		//AxmMovePos(0, 0, 50, 200, 200);
	}
}








#endif