#include "sendMessagesUR5.h"

void sendJointAngles(SOCKET sock, double jointAngles[])
{
	stringstream temp;
	string cmd;
	//movej(q, a=1.4, v=1.05, t=0, r=0)
	temp << "def f():\n"
		<< "movej([" << jointAngles[0] << "," << jointAngles[1] << "," << jointAngles[2] << ","
		<< jointAngles[3] << "," << jointAngles[4] << "," << jointAngles[5] << "], a=1.5, v=1.1)\n"
		<< "end\n";
	cmd = temp.str();
	cmd += '\n';
	int iRet = send(sock, cmd.c_str(), (int)cmd.length(), 0);
	if (iRet == SOCKET_ERROR)
	{
		int iErrCode = WSAGetLastError();
		cout << "Send Joint Angles Error!\n";
	}
}

void sendTcpPos(SOCKET sock, double tcpPos[], int moveType = MOVEJ)
{
	stringstream temp;
	string cmd;
	if (moveType == MOVEJ)
	{
		temp << "def f():\n"
			<< "movej(get_inverse_kin(p[" << tcpPos[0] << "," << tcpPos[1] << "," << tcpPos[2] << ","
			<< tcpPos[3] << "," << tcpPos[4] << "," << tcpPos[5] << "]), a=0.3, v=0.2)\n"
			<< "end\n";
	}
	else
	{
		temp << "def f():\n"
			<< "movel(p[" << tcpPos[0] << "," << tcpPos[1] << "," << tcpPos[2] << ","
			<< tcpPos[3] << "," << tcpPos[4] << "," << tcpPos[5] << "], a=0.3, v=0.2)\n"
			<< "end\n";
	}
	
	cmd = temp.str();
	cmd += '\n';
	int iRet = send(sock, cmd.c_str(), (int)cmd.length(), 0);
	if (iRet == SOCKET_ERROR)
	{
		int iErrCode = WSAGetLastError();
		cout << "Send TcpPos  Error!\n";
	}
}