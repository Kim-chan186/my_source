#pragma once
#ifndef COMMUNICAION_H
#define COMMUNICAION_H

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <WinSock2.h>

#pragma comment(lib, "ws2_32")

#include <string>
#include <cstring>
////////

#define PORT		10048
#define DELTA_EPOCH_IN_MICROSECS 11644473600000000ULL

using namespace std;


////////

WSADATA wsaData;
SOCKET cSocket;
SOCKADDR_IN cADDR = {};
WSADATA wsaData2;
SOCKET cSocket2;
SOCKADDR_IN cADDR2 = {};
string IP1 = "192.168.0.3 "; //순혁이형 노트북 "192.168.0.3"  //예린이 노트북	"192.168.0.22";

void udpClient(double _x, double _y, double _z)
{
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	cSocket = socket(PF_INET, SOCK_DGRAM, 0);

	memset(&cADDR, 0, sizeof(cADDR));
	cADDR.sin_family = AF_INET;
	cADDR.sin_addr.s_addr = inet_addr(IP1.c_str());
	cADDR.sin_port = htons(PORT);
	connect(cSocket, (SOCKADDR *)&cADDR, sizeof(cADDR));

	char send1[35] = "\0",
		 send2[3] = "\0",
		 send3[10] = "\0";

	//string IP;
	//if (num == 1) {
	//	IP = "192.168.0.7"; //데탑
	//}
	//else if (num == 2) {
	//	IP = "192.168.0.11"; //순혁이형 노트북
	//}
	//else if (num == 3) {
	//	IP = "192.168.0.22"; //예린이 노트북
	//}
	//else if (num == 4) {
	//	IP = "192.168.0.12"; //지수 노트북
	//}
	//else {
	//	cout << "udpClient 잘못입력하였";
	//}


	_gcvt(_x, 5, send1);
	if (_y > 0) {
		_gcvt(1, 3, send2);
	}
	else {
		_gcvt(0, 7, send2);
	}
	_gcvt(_y, 5, send2);
	_gcvt(_z+0.5, 7, send3);

	strcat(send1, " ");
	strcat(send1, send2);
	strcat(send1, " ");
	strcat(send1, send3);
	
	//cout << send1 << endl;

	sendto(cSocket, send1, 25, 0, (SOCKADDR *)&cADDR, sizeof(cADDR));

	closesocket(cSocket);
	WSACleanup();
}

#endif