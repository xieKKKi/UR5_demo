#pragma once
#include <WinSock2.h>
#include <sstream>
#include <iostream>
using namespace std;

#define MOVEJ 0
#define MOVEL 1

//movej��ʽ�ƶ������˸��ؽڵ�ָ���Ĺؽ�λ��
void sendJointAngles(SOCKET sock, double jointAngles[]);

//movej&movel��ʽ�ƶ�������TCP��ָ��λ��
void sendTcpPos(SOCKET sock, double tcpPos[], int moveType);