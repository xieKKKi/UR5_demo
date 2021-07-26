#include "receiveMessagesUR5.h"

//由于UR返回数据为Big-Endian，而计算机中的数据为Little-Endian，必须进行数据字节转换，所以编了以下两个函数完成


double GetDouble(PBYTE pData)
{
	double t;
	PBYTE p = (PBYTE)&t;
	int i;
	for (i = 0; i < 8; i++)
	{
		p[i] = pData[7 - i];
	}
	return t;
}

//实际上GetDword函数和htonl()函数功能一样
DWORD GetDword(PBYTE pData)
{
	DWORD t;
	PBYTE p = (PBYTE)&t;
	int i;
	for (i = 0; i < 4; i++)
	{
		p[i] = pData[3 - i];
	}
	return t;
}

//按照UR机器人的30003端口返回数据格式进行解析
double OnRecvData(char* pData, int &nLen, double jointAngles[], double tcpPos[])
{
	DWORD messageSize;
	messageSize = GetDword((PBYTE)pData);
	//printf("Message Size: %d\n", messageSize);
	nLen = messageSize;

	//double jointAngles[6];
	for (int n = 0; n < 6; n++)
	{
		jointAngles[n] = GetDouble((PBYTE)(pData + 252 + n * 8));
		//printf("Joint Angles%d: %f ", n, jointAngles[n]);
	}
	//printf("\n");

	//double tcpPos[6];
	for (int n = 0; n < 6; n++)
	{
		tcpPos[n] = GetDouble((PBYTE)(pData + 444 + n * 8));
		//printf("TCP Pos%d: %f ",n , tcpPos[n]);
	}
	//printf("\n");

	double safetyMode = 0;
	safetyMode = GetDouble((PBYTE)(pData + 812));
	return safetyMode;
}

int checkRecvData_len(SOCKET sock)
{
	char* recvBuf = new char[MSGSIZE];
	memset(recvBuf, 0, MSGSIZE);
	int ret = recv(sock, recvBuf, MSGSIZE, 0);
	DWORD messageSize;
	if (ret > 0)
	{
		messageSize = GetDword((PBYTE)recvBuf);
	}
	else
	{
		printf("Receive error!!");
	}
	delete[]recvBuf;
	if (messageSize != MSGSIZE)
	{
		printf("Message Size Correct: %d\n", messageSize);  //提示要修改MSGSIZE
	}
	return messageSize;
}

void RecvData_jointAngles(char* pData, double jointAngles[])
{
	for (int n = 0; n < 6; n++)
	{
		jointAngles[n] = GetDouble((PBYTE)(pData + 252 + n * 8));
		//printf("Joint Angles%d: %f ", n, jointAngles[n]);
	}
	//printf("\n");
}

void RecvData_tcpPos(char* pData, double tcpPos[])
{
	for (int n = 0; n < 6; n++)
	{
		tcpPos[n] = GetDouble((PBYTE)(pData + 444 + n * 8));
		//printf("TCP Pos%d: %f ",n , tcpPos[n]);
	}
	//printf("\n");
}

int RecvData_safetyMode(char* pData)
{
	int safetyMode = 0;
	safetyMode = (int)GetDouble((PBYTE)(pData + 812));
	switch (safetyMode)
	{
	case SAFETY_MODE_UNDEFINED_SAFETY_MODE:
		cout << "SAFETY_MODE_UNDEFINED_SAFETY_MODE" << '\n';
		break;
	case SAFETY_MODE_VALIDATE_JOINT_ID:
		cout << "SAFETY_MODE_VALIDATE_JOINT_ID" << '\n';
		break;
	case SAFETY_MODE_FAULT:
		cout << "SAFETY_MODE_FAULT" << '\n';
		break;
	case SAFETY_MODE_VIOLATION:
		cout << "SAFETY_MODE_VIOLATION" << '\n';
		break;
	case SAFETY_MODE_ROBOT_EMERGENCY_STOP:
		cout << "SAFETY_MODE_ROBOT_EMERGENCY_STOP" << '\n';
		break;
	case SAFETY_MODE_SYSTEM_EMERGENCY_STOP:
		cout << "SAFETY_MODE_SYSTEM_EMERGENCY_STOP" << '\n';
		break;
	case SAFETY_MODE_SAFEGUARD_STOP:
		cout << "SAFETY_MODE_SAFEGUARD_STOP" << '\n';
		break;
	case SAFETY_MODE_RECOVERY:
		cout << "SAFETY_MODE_RECOVERY" << '\n';
		break;
	case SAFETY_MODE_PROTECTIVE_STOP:
		cout << "SAFETY_MODE_PROTECTIVE_STOP" << '\n';
		break;
	case SAFETY_MODE_REDUCED:
		cout << "SAFETY_MODE_REDUCED" << '\n';
		break;
	default: //normal
		break;
	}
	return safetyMode;
}

bool checkRecvData_speed(SOCKET sock)
{
	int nLen = MSGSIZE;
	char* recvBuf = new char[nLen];
	memset(recvBuf, 0, nLen);
	while (true)
	{ 
		int ret = recv(sock, recvBuf, nLen, 0);
		if (ret > 0)
		{
			double jointSpeed[6];
			for (int n = 0; n < 6; n++)
			{
				jointSpeed[n] = GetDouble((PBYTE)(recvBuf + 300 + n * 8));
				printf("Joint Speed%d: %f ", n, jointSpeed[n]);
			}
			printf("\n");

			if (fabs(jointSpeed[0]) <= 0.000002 && fabs(jointSpeed[1]) <= 0.000002 &&
				fabs(jointSpeed[2]) <= 0.000002 &&fabs(jointSpeed[3]) <= 0.000002 &&
				fabs(jointSpeed[4]) <= 0.000002 &&fabs(jointSpeed[5]) <= 0.000002)
			{
				break;
			}
		}
		else
		{
			printf("Receive error!!");
			return false;
		}
	}
	delete[]recvBuf;
	return true;
}