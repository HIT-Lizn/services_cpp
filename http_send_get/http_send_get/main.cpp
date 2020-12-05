#include "my_class.h"



using namespace std;

int main(int argc, char* argv[]) 
{
	if (argc != PARAGRAM_NUMBER) 
	{
		return ERROR_WRONG_PARAGRAM_NUMBER;
	}
	/*string url = "";
	Myhttp myhttp = Myhttp(url);
	int getresult = myhttp.get();
	return getresult;*/
	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);

	sockaddr_in sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));
	sockAddr.sin_family = AF_INET;
	//struct in_addr dst;
	sockAddr.sin_addr.s_addr = inet_pton(AF_INET, "14.215.177.39", NULL);
	//sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	sockAddr.sin_port = htons(443);

	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	connect(sock, (SOCKADDR*)&sockAddr, sizeof(sockAddr));

	char infoBuff[MAXBYTE] = { 0 };
	recv(sock, infoBuff, MAXBYTE, NULL);

	cout << "从服务器接受到得信息为：" << infoBuff << endl;

	closesocket(sock);
	WSACleanup();
	return 0;
}