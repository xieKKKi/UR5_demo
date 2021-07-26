
#include <WinSock2.h>
#include <sstream>
#include <iostream>

#include "socketConnection.h"
#include "receiveMessagesUR5.h"
#include "sendMessagesUR5.h"
using namespace std;

//ͬ��������ʵ��Ĺؽڽ�
void synchronize(SOCKET sockUR5sim, SOCKET sockUR5, string ipUR5sim, string ipUR5)
{
	closesocket(sockUR5);  //û�м�ʱrecv�����ݻᱣ�浽������������ͬ�������ݲ������µġ�������������ȡ���µ����ݡ���û�и��õİ취��
	sockUR5 = initConnection(ipUR5);
	int nLen = MSGSIZE;
	char* recvBuf = new char[nLen];
	memset(recvBuf, 0, nLen);
	int ret = recv(sockUR5, recvBuf, nLen, 0);
	if (ret > 0)
	{
		double jointAngles[6];
		RecvData_jointAngles(recvBuf, jointAngles);
		sendJointAngles(sockUR5sim, jointAngles);
	}
	else
	{
		printf("Receive error!!");
	}
	cout << "Synchronizing..." << '\n';
	Sleep(100);
	closesocket(sockUR5sim);  //û�м�ʱrecv�����ݻᱣ�浽������������ͬ�������ݲ������µġ�������������ȡ���µ����ݡ���û�и��õİ취��
	sockUR5sim = initConnection(ipUR5sim);
	checkRecvData_speed(sockUR5sim);

	ret = recv(sockUR5sim, recvBuf, nLen, 0);
	if (ret > 0)
	{
		double safetyMode = RecvData_safetyMode(recvBuf);
		if (safetyMode != SAFETY_MODE_NORMAL)
		{
			cout << "ERROR occurred! " << '\n';
			cout << "�������������̬���ٴ�ͬ����" << '\n';
		}
		else
		{
			cout << "Synchronize completed!" << '\n';
		}
	}
	else
	{
		printf("Receive error!!");
	}
	delete[]recvBuf;
}

//�滮����·��
void plan(SOCKET sockUR5sim, string ipUR5sim, double targetTcpPos[], int moveType)
{
	sendTcpPos(sockUR5sim, targetTcpPos, moveType);
	cout << "Planning..." << '\n';
	Sleep(100);
	closesocket(sockUR5sim);  //û�м�ʱrecv�����ݻᱣ�浽������������ͬ�������ݲ������µġ�������������ȡ���µ����ݡ���û�и��õİ취��
	sockUR5sim = initConnection(ipUR5sim);
	checkRecvData_speed(sockUR5sim);

	int nLen = MSGSIZE;
	char* recvBuf = new char[nLen];
	memset(recvBuf, 0, nLen);
	int ret = recv(sockUR5sim, recvBuf, nLen, 0);
	if (ret > 0)
	{
		double safetyMode = RecvData_safetyMode(recvBuf);
		if (safetyMode != SAFETY_MODE_NORMAL)
		{
			cout << "ERROR occurred! " << '\n';
			cout << "�������������̬���ٴ�ͬ�������¹滮��" << '\n';
		}
		else
		{
			cout << "Plan completed!" << '\n';
		}
	}
	else
	{
		printf("Receive error!!");
	}

	delete[]recvBuf;
}

//ʵ��ִ��
void execute(SOCKET sockUR5, string ipUR5, double targetTcpPos[], int moveType)
{
	sendTcpPos(sockUR5, targetTcpPos, moveType);
	cout << "Executing..." << '\n';
	Sleep(100);
	closesocket(sockUR5);  //û�м�ʱrecv�����ݻᱣ�浽������������ͬ�������ݲ������µġ�������������ȡ���µ����ݡ���û�и��õİ취��
	sockUR5 = initConnection(ipUR5);
	checkRecvData_speed(sockUR5);

	int nLen = MSGSIZE;
	char* recvBuf = new char[nLen];
	memset(recvBuf, 0, nLen);
	int ret = recv(sockUR5, recvBuf, nLen, 0);
	if (ret > 0)
	{
		double safetyMode = RecvData_safetyMode(recvBuf);
		if (safetyMode != SAFETY_MODE_NORMAL)
		{
			cout << "ERROR occurred! " << '\n';
			cout << "�������������̬���ٴ�ͬ�������¹滮��ִ�С�" << '\n';
		}
		else
		{
			cout << "Execute completed!" << '\n';
		}
	}
	else
	{
		printf("Receive error!!");
	}
	delete[]recvBuf;
}

void mainLoop(SOCKET sockUR5sim, SOCKET sockUR5, string ipUR5sim, string ipUR5)
{
	int key = 0;
	double targetTcpPos[] = { -0.739, -0.592, 0.343 , 1.374 ,2.390 , -2.505 };
	while (key != 4)
	{
		cout << '\n' << "**************ѡ��Ҫִ�еĲ���**************" << '\n';
		cout << "1: Synchronize; 2: Plan; 3: Execute; 4: Quit" << '\n';
		cin >> key;
		switch (key)
		{
		case 1:
			synchronize(sockUR5sim, sockUR5, ipUR5sim, ipUR5);
			break;
		case 2:
			plan(sockUR5sim, ipUR5sim, targetTcpPos, MOVEJ);
			break;
		case 3:
			execute(sockUR5, ipUR5, targetTcpPos, MOVEJ);
			break;
		default:
			break;
		}
	}
}
int main()
{
	initWSA();

	string ipUR5sim = "192.168.89.130";
	//string ipUR5 = "192.168.10.158";
	string ipUR5 = "192.168.89.130";
	SOCKET sockUR5sim = initConnection(ipUR5sim);
	SOCKET sockUR5 = initConnection(ipUR5);
	cout << "sockUR5sim data len:" << checkRecvData_len(sockUR5sim)<<'\n';
	cout << "sockUR5 data len:" << checkRecvData_len(sockUR5)<<'\n';
	if (checkRecvData_len(sockUR5sim) != MSGSIZE)
	{
		return 0;
	}
	if (checkRecvData_len(sockUR5) != MSGSIZE)
	{
		return 0;
	}

	mainLoop(sockUR5sim, sockUR5, ipUR5sim, ipUR5);
	
	closesocket(sockUR5sim);
	closesocket(sockUR5);
	WSACleanup();

	return 0;
}