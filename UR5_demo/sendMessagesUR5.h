#pragma once
#include <WinSock2.h>
#include <sstream>
#include <iostream>
using namespace std;

#define MOVEJ 0
#define MOVEL 1

//movej方式移动机器人各关节到指定的关节位置
void sendJointAngles(SOCKET sock, double jointAngles[]);

//movej&movel方式移动机器人TCP到指定位姿
void sendTcpPos(SOCKET sock, double tcpPos[], int moveType);