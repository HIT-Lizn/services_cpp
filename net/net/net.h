#pragma once
#include<iostream>
#include<winsock.h>
#include<pthread.h>
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib, "pthreadVC2.lib")

using namespace std;

class net
{
public:
	virtual int my_send(string s, string ip, int port) = 0;
	virtual string my_recive() = 0;
};

class tcp :public net
{
private:
	//WSADATA wsaData;
	SOCKADDR_IN my_addr;
	SOCKET my_sock;

public:
	tcp(string ip, int port, int max);
	int my_send(string s, string ip, int port);
	string my_recive();
	~tcp();
};

class udp :public net
{
private:
	SOCKADDR_IN my_addr;
	SOCKET my_sock;

public:
	udp(string ip, int port);
	int my_send(string s, string ip, int port);
	string my_recive();
};
