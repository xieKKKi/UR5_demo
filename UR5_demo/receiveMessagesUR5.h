#pragma once
#include <WinSock2.h>
#include <sstream>
#include <iostream>
using namespace std;
#define MSGSIZE 1220

#define SAFETY_MODE_UNDEFINED_SAFETY_MODE 11
#define SAFETY_MODE_VALIDATE_JOINT_ID 10
#define SAFETY_MODE_FAULT 9
#define SAFETY_MODE_VIOLATION 8
#define SAFETY_MODE_ROBOT_EMERGENCY_STOP 7
#define SAFETY_MODE_SYSTEM_EMERGENCY_STOP 6
#define SAFETY_MODE_SAFEGUARD_STOP 5
#define SAFETY_MODE_RECOVERY 4
#define SAFETY_MODE_PROTECTIVE_STOP 3
#define SAFETY_MODE_REDUCED 2
#define SAFETY_MODE_NORMAL 1

//由于UR返回数据为Big-Endian，而计算机中的数据为Little-Endian，必须进行数据字节转换，所以编了以下两个函数完成
double GetDouble(PBYTE pData);
//实际上GetDword函数和htonl()函数功能一样
DWORD GetDword(PBYTE pData);
//按照UR机器人的30003端口返回数据格式进行解析
double OnRecvData(char* pData, int &nLen, double jointAngles[], double tcpPos[]);
int checkRecvData_len(SOCKET sock);  //检查数据包长度是不是MSGSIZE
void RecvData_jointAngles(char* pData, double jointAngles[]);  //关节角
void RecvData_tcpPos(char* pData, double tcpPos[]);  //tcp位姿
int RecvData_safetyMode(char* pData);  //安全模式，检查是否有报错
bool checkRecvData_speed(SOCKET sock);  //关节是否还在运动, 返回值false表示出现异常