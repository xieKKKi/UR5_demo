#pragma once
#include <WinSock2.h>
#include <Windows.h>
#include <sstream>
#include <iostream>
#include <ws2tcpip.h> 
#pragma comment(lib,"ws2_32.lib")  

using namespace std;

bool initWSA();
SOCKET initConnection(string ip);