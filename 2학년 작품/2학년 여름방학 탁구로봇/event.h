#pragma once
#ifndef EVENT_H
#define EVENT_H

#include <opencv2/opencv.hpp>
#include <string>

using namespace std;



void msg() {
	char c_str[35];
	int cont = 0,
		inKey = 0;
	while (1) {
		inKey = waitKeyEx(1);

		if (inKey == 13) // Enter
		{
			int num = atoi(c_str);
			printf("  //msg\n");
			//udpClient(15, num);
			return;
		}
		else if (inKey != -1) {
			*(c_str + cont++) = inKey;
			printf("%c", inKey);
		}
	}
}
void keybd_event(int& inKey, double& event_val1, int& cell_flag) {
	//event_val1 : alpha

	switch (inKey) {
	case 32:       	// SpaceBar: 32
		cout << "\n               -----<   STOP  >-----\n\n";

		do {
			inKey = waitKeyEx(1);
		} while (inKey != 32);

		cout << "\n               -----< ReStart >-----\n\n";
		break;

	case 2490368:		//왼쪽: 2424832      위 : 2490368      오른쪽 : 2555904      아래 : 2621440
		if (event_val1 == 1) {}
		else     event_val1 += 0.05;

		break;//위

	case 2621440:
		if (event_val1 == 0) {}
		else     event_val1 -= 0.05;

		break;//아래

	case -1:       		// no input
		break;

	case 115:       	// s
		cell_flag = 1;
		break;
	case 112:       	// p
		cell_flag = 2;
		break;

	case 109:       	// m
		msg();
		break;
	case 98:		 	// b
		throw_ball(1);
		break;

	default:
		cout << "get key : " << inKey << endl;
		break;

	}
}





#endif