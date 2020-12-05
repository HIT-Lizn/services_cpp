#pragma once
#include "iostream"
#include "const.h"
#include <string>
#include "winsock2.h"
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")  /* WinSockʹ�õĿ⺯�� */

using namespace std;

class Myhttp 
{
public:
	Myhttp(string url);
	~Myhttp();

	int send();
	int get();

private:
	string url;
	SOCKET msocket;
};