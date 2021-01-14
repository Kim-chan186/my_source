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
		DWORD Code = AxlOpen(7);   //���� �� ���̺귯�� �ʱ�ȭ IRQ ��ȣ�� �ʱ⿡ 7�� ����
		if (Code == AXT_RT_SUCCESS)
		{
			//printf("���̺귯���� �ʱ�ȭ�Ǿ����ϴ�.\n");
			//��� ����� �ִ��� �˻�
			DWORD uStatus;
			Code = AxmInfoIsMotionModule(&uStatus);   // ���� �ý��ۿ� �����Ǿ��ִ� ��� ����� ���� ���� Ȯ��
			if (Code == AXT_RT_SUCCESS)   // ���̺귯�� �ʱ�ȭ�� �������� ��
			{
				//printf("���̺귯���� �ʱ�ȭ�Ǿ����ϴ�.\n");
				if (uStatus == STATUS_EXIST)
				{
					printf("���̺귯���� �ʱ�ȭ�Ǿ����ϴ�.\n");

					AxmMotLoadParaAll(mot_file);   //mot ���Ͽ� �����ִ� ���� �������� �ҷ���

					// ���� ��ġ�� �������� ����
					AxmStatusSetActPos(0, 0.0);
					AxmStatusSetCmdPos(0, 0.0);

					AxmSignalServoOn(0, ENABLE);// ���� ON ä�� ��ȣ 0�� enable

					AxmMotSetAbsRelMode(0, 0); //0->abs, 1->Rel ä�ι�ȣ 0, ��ġ ���� ����� : �̵��Լ����� ����ϴ� ��ġ���� ������ġ���� �̵��� �̵����� �ȴ�.
					AxmMotSetProfileMode(0, 3);   //0->symetric trapezode, 1->unsymetric trapezode, 2->reserved, 3->symetric S Curve, 4->unsymetric S Cuve
				}
			}
		}
	}
	~CLinear_actu()
	{
		AxmSignalServoOn(0, 0);   // ���� off
		AxlClose();   //�޸� ��ȯ
	}

	//void move_actu(double pos)
	//{
	//	if (pos <= 0)
	//		pos += _Y;
	//	else
	//		pos -= _Y;
	//	if (pos > max_position) {   // ���Ͼ ������ ���� �ȿ� ���� �� ���� 
	//		pos = max_position;
	//	}
	//	else if (pos < -max_position) {
	//		pos = -max_position;
	//	}
//
	//	if ((pos < max_position) & (pos > -max_position)) {   // ���Ͼ ������ ���� �ȿ� ���� �� ���� 
	//		AxmMovePos(0, pos, vel, accel, dccel);  // ����� ������ ���� �Լ� �޽������ ���ᰡ �Ǿ�� �Լ�Ż�� 
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




		if (pos > max_position) {   // ���Ͼ ������ ���� �ȿ� ���� �� ���� 
			pos = max_position;
		}
		else if (pos < -max_position) {
			pos = -max_position;
		}


		if ((pos <= max_position) & (pos >= -max_position)) {   // ���Ͼ ������ ���� �ȿ� ���� �� ���� 
			AxmMoveStartPos(0, pos, vel, accel, dccel);  // ����� ������ ���� �Լ� �޽������ ���ᰡ �Ǿ�� �Լ�Ż��  �� ��������(Ȳ����) �������δ� �ؿ� ������ �̰Ͷ��� �ȵɵ�
		}
		//AxmMovePos(0, 0, 1000, 500, 500);
	}





	double get_act_pose(){
		double dPos;
		AxmStatusGetActPos(0, &dPos);   // ä�� ��ȣ0, ��ġ
		return dPos;   //���ڴ��� �о���� Actual ��ġ�� ��ȯ �Ѵ�.
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
	//		printf("�����ض� �Ѥ� �Է¹��� ������ : %f \n", position);
	//		
	//		while (now_pos == position); // ����� ��� ���´� ���� ������������ �����ؾ��� ��� �񱳸� �ϸ鼭 �ٸ��� �������� �Ѿ
	//		//while (true) {
	//		//	unique_lock<mutex>lock(mut);
	//		//	while (!linear_flag)
	//		//		signal.wait(lock);
	//		//	linear_flag = false;
	//		now_pos = *pos;				// ���ο� ����� ���� ����� ����
	//		printf("now_pos : %.1f\n", now_pos);
	//		printf("pre_pos : %.1f\n", pre_pos);
	//
	//		if ((pre_pos * now_pos) < 0) { // ���� ��ǥ��ġ�� ���� ��ǥ��ġ�� ������ �ݴ��� ��
	//			AxmMoveEStop(0); // ��� ����
	//			printf("Emergency STOP\n");
	//			AxmMoveStartPos(0, *pos, vel, accel, dccel);
	//		}
	//		else {
	//			AxmStatusReadInMotion(0, &uStatus);
	//			if (uStatus) { // True �̸� ������   
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

		printf("�Է¹��� posiiton = %.1f\n", position);
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
	//		if ((pre_pos * now_pos) < 0) { // ���� ��ǥ��ġ�� ���� ��ǥ��ġ�� ������ �ݴ��� ��
	//			AxmMoveEStop(0); // ��� ����
	//			AxmMoveStartPos(0, *pos, vel, accel, dccel);
	//		}
	//		else {
	//			if (!(AxmStatusReadMotion(0, &uStatus))) { // 0 �̸� ������
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
		if (position > max_position) {   // ���Ͼ ������ ���� �ȿ� ���� �� ���� 
			position = max_position;
		}
		else if (position < -max_position) {
			position = -max_position;
		}
		AxmMovePos(0, position, vel, accel, dccel);  // ����� ������ ���� �Լ� �޽������ ���ᰡ �Ǿ�� �Լ�Ż��
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

			if (direction_flag != pre_direction_flag) { // ���� ��ǥ��ġ�� ���� ��ǥ��ġ�� ������ �ݴ��� ��
				//AxmMoveSStop(0); // ��� ���� ����
				AxmMoveEStop(0);
				AxmMoveStartPos(0, position, vel, accel, dccel);
				//printf("���� �ٲ� Emergency STOP\n\n");
			}
			else {
				AxmStatusReadInMotion(0, &uStatus);
				if (uStatus) { // True �̸� ������   
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
//			if (direction_flag != pre_direction_flag) { // ���� ��ǥ��ġ�� ���� ��ǥ��ġ�� ������ �ݴ��� ��
//				AxmMoveEStop(0);
//				AxmMoveStartPos(0, position, vel, accel, dccel);
//				//printf("���� �ٲ� Emergency STOP\n\n");
//			}
//			else {
//				AxmStatusReadInMotion(0, &uStatus);
//				if (uStatus) { // True �̸� ������   
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
		if (cmd_position > max_position) {   // ���Ͼ ������ ���� �ȿ� ���� �� ���� 
			cmd_position = max_position;
		}
		else if (cmd_position < -max_position) {
			cmd_position = -max_position;
		}

		if (cmd_position <= 0)		cmd_position += 25;
		else						cmd_position -= 25;

		if ((cmd_position < max_position) & (cmd_position > -max_position)) {   // ���Ͼ ������ ���� �ȿ� ���� �� ���� 
			AxmMovePos(0, cmd_position, vel, accel, dccel);  // ����� ������ ���� �Լ� �޽������ ���ᰡ �Ǿ�� �Լ�Ż��  �� ��������(Ȳ����) �������δ� �ؿ� ������ �̰Ͷ��� �ȵɵ�
		}
		Sleep(18);
		position = 25;
		//AxmMovePos(0, 0, 50, 200, 200);
	}
}








#endif