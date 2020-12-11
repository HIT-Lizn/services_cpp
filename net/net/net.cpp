#include"net.h"

tcp::tcp(string ip, int port, int max)
{
	//WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (ip == "")
	{
		my_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	}
	else
	{
		my_addr.sin_addr.S_un.S_addr = inet_addr(ip.c_str());
	}
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(port);
	my_sock = socket(AF_INET, SOCK_STREAM, 0);
	bind(my_sock, (SOCKADDR*)&my_addr, sizeof(SOCKADDR));
	listen(my_sock, max);
}

int tcp::my_send(string s, string ip, int port)
{
	SOCKET my_send;
	SOCKADDR_IN send_addr;

	my_send = socket(AF_INET, SOCK_STREAM, 0);

	send_addr.sin_addr.S_un.S_addr = inet_addr(ip.c_str());
	send_addr.sin_family = AF_INET;
	send_addr.sin_port = htons(port);

	if (connect(my_send, (SOCKADDR*)&send_addr, sizeof(SOCKADDR)))
	{
		cout << "failed to connect server!" << endl;
		return -1;
	}
	cout << "successed to connect to server!" << endl;

	const char* message = s.c_str();
	if (send(my_send, message, strlen(message) + 1, 0) > 0)
	{
		cout << "link was broken!" << endl;
	}
	
	closesocket(my_send);
	return 0;
}

string tcp::my_recive()
{
	SOCKET sockClient;
	SOCKADDR_IN addrClient;

	int len = sizeof(SOCKADDR);
	char recvBuf[10000];
	
	while (1)
	{
		sockClient = accept(my_sock, (SOCKADDR*)&addrClient, &len);
		if (sockClient)
		{
			while (recv(sockClient, recvBuf, 10000, 0) > 0)
			{
				cout << recvBuf << " port:" << addrClient.sin_port << " address:" << inet_ntoa(addrClient.sin_addr) <<endl;
			}
			cout << "this link was broken!" << endl;
		}
	}

	closesocket(sockClient);
	return "";
}

tcp::~tcp()
{
	//WSACleanup();
}

udp::udp(string ip, int port)
{
	if (ip == "")
		my_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	else
		my_addr.sin_addr.S_un.S_addr = inet_addr(ip.c_str());
	my_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(port);
	bind(my_sock, (SOCKADDR*)&my_addr, sizeof(SOCKADDR));
}

string udp::my_recive()
{
	char RecvBuf[1024];
	int BufLen = 1024;
	sockaddr_in SenderAddr;
	int SenderAddrSize = sizeof(SenderAddr);
	bind(my_sock, (SOCKADDR*)&my_addr, sizeof(SOCKADDR));
	while (1)
	{
		recvfrom(my_sock, RecvBuf, BufLen, 0, (SOCKADDR*)&SenderAddr, &SenderAddrSize);
		cout << RecvBuf << " port:" << SenderAddr.sin_port << " address:" << inet_ntoa(SenderAddr.sin_addr) << endl;
	}
	closesocket(my_sock);
	return "";
}

int udp::my_send(string s, string ip, int port)
{
	SOCKET SendSocket;
	sockaddr_in RecvAddr;
	const char* message = s.c_str();
	int BufLen = 1024;

	SendSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	RecvAddr.sin_family = AF_INET;
	RecvAddr.sin_port = htons(port);
	RecvAddr.sin_addr.s_addr = inet_addr(ip.c_str());
	sendto(SendSocket, message, strlen(message) + 1, 0, (SOCKADDR*)&RecvAddr, sizeof(RecvAddr));
	closesocket(SendSocket);
	return 0;
}