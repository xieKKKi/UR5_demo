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

//����UR��������ΪBig-Endian����������е�����ΪLittle-Endian��������������ֽ�ת�������Ա������������������
double GetDouble(PBYTE pData);
//ʵ����GetDword������htonl()��������һ��
DWORD GetDword(PBYTE pData);
//����UR�����˵�30003�˿ڷ������ݸ�ʽ���н���
double OnRecvData(char* pData, int &nLen, double jointAngles[], double tcpPos[]);
int checkRecvData_len(SOCKET sock);  //������ݰ������ǲ���MSGSIZE
void RecvData_jointAngles(char* pData, double jointAngles[]);  //�ؽڽ�
void RecvData_tcpPos(char* pData, double tcpPos[]);  //tcpλ��
int RecvData_safetyMode(char* pData);  //��ȫģʽ������Ƿ��б���
bool checkRecvData_speed(SOCKET sock);  //�ؽ��Ƿ����˶�, ����ֵfalse��ʾ�����쳣