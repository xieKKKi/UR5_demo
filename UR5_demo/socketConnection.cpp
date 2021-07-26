#include "socketConnection.h"

bool initWSA()
{
	WSADATA wsaData;  //≥ı ºªØWSA
	WORD wVersionRequested = MAKEWORD(2, 2);
	if (WSAStartup(wVersionRequested, &wsaData) != 0)
	{
		printf("Init WSA Failed!\n");
		return false;
	}
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
	{
		WSACleanup();
		printf("Init WSA Failed!\n");
		return false;
	}
	return true;
}

SOCKET initConnection(string ip)
{
	SOCKET sockSli;
	sockSli = socket(AF_INET, SOCK_STREAM, 0);
	SOCKADDR_IN addrSer;
	addrSer.sin_family = AF_INET;
	addrSer.sin_port = htons(30003);
	inet_pton(AF_INET, ip.c_str(), &addrSer.sin_addr.S_un.S_addr);
	int res = connect(sockSli, (SOCKADDR*)&addrSer, sizeof(SOCKADDR));
	if (res != 0)
	{
		cout << ip<< ": Client Connect Servrer Fail!" << endl;
		return 0;
	}
	else
	{
		cout << ip << ": Client Connect Servrer Success!" << endl;
	}
	return sockSli;
}
