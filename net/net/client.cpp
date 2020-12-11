#include<iostream>
#include<winsock.h>
#include<pthread.h>
#include"net.h"
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib, "pthreadVC2.lib")

using namespace std;

pthread_mutex_t mu1;

void* recevie(void* args)
{
	WSADATA wsaData;

	SOCKET sockServer;
	SOCKADDR_IN addrServer;

	SOCKET sockClient;
	SOCKADDR_IN addrClient;

	WSAStartup(MAKEWORD(2, 2), &wsaData);
	sockServer = socket(AF_INET, SOCK_STREAM, 0);

	addrServer.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrServer.sin_family = AF_INET;
	addrServer.sin_port = htons(6000);

	bind(sockServer, (SOCKADDR*)&addrServer, sizeof(SOCKADDR));
	listen(sockServer, 5);

	int len = sizeof(SOCKADDR);
	char recvBuf[10000];
	while (1)
	{
		sockClient = accept(sockServer, (SOCKADDR*)&addrClient, &len);
		if (sockClient)
		{
			while (recv(sockClient, recvBuf, 10000, 0) > 0)
			{
				cout << recvBuf << endl;
			}
			cout << "this link was broken!" << endl;

			closesocket(sockClient);
		}
	}
	
	WSACleanup();
	
	pthread_exit(NULL);
	return 0;
}

void* send(void* args)
{
	WSADATA wsaData;

	SOCKET sockClient;
	SOCKADDR_IN addrServer;

	WSAStartup(MAKEWORD(2, 2), &wsaData);

	sockClient = socket(AF_INET, SOCK_STREAM, 0);

	addrServer.sin_addr.S_un.S_addr = inet_addr("10.236.13.236");
	addrServer.sin_family = AF_INET;
	addrServer.sin_port = htons(6000);

	if (connect(sockClient, (SOCKADDR*)&addrServer, sizeof(SOCKADDR)))
	{
		cout << "failed to connect server!" << endl;
		pthread_exit(NULL);
		return 0;
	}
	cout << "successed to connect to server!" << endl;

	string str;
	cin >> str;
	const char* message = str.c_str();

	while (str != "quit" && send(sockClient, message, strlen(message) + 1, 0) > 0)
	{
		cin >> str;
		message = str.c_str();
	}
	cout << "link was broken!" << endl;

	closesocket(sockClient);

	WSACleanup();

	pthread_exit(NULL);
	return 0;
}

int k = 0;

void* a(void* t)
{
	//WSADATA wsaData;
	//WSAStartup(MAKEWORD(2, 2), &wsaData);
	//net* nptr1 = new udp("", 7000);//tcp("", 6000, 5);
	//nptr1->my_recive();
	//WSACleanup();
	pthread_mutex_lock(&mu1);
	for (int i = 0; i < 10; i++)
	{
		Sleep(1000);
		cout << "a:" << k << endl;
		k++;
	}
	pthread_mutex_unlock(&mu1);
	pthread_exit(NULL);
	return 0;
}

void* b(void* t)
{
	//WSADATA wsaData;
	//WSAStartup(MAKEWORD(2, 2), &wsaData);
	//net* nptr2 = new udp("10.236.13.236", 6001);//tcp("10.236.13.236", 6001, 6);
	//while (1)
	//{
	//	nptr2->my_send("nmzl!!!!!!", "10.236.13.236", 7000);
	//}
	//
	//WSACleanup();
	pthread_mutex_lock(&mu1);
	for (int i = 0; i < 10; i++)
	{
		Sleep(1000);
		cout << k << endl;
		k--;
	}
	pthread_mutex_unlock(&mu1);
	pthread_exit(NULL);
	return 0;
}

pthread_mutex_t mutex;
pthread_mutex_t mutex2;
pthread_cond_t cond;
pthread_cond_t cond2;
int i = 0;                //共享资源
void child1(char* arg)
{
	Sleep(1000);
	while (1)
	{
		pthread_mutex_lock(&mutex);
	
		i++;
		
		if (i % 5 != 0)
		{
			printf("我是线程  1  打印的数都非5的倍数:  %d \n", i);
			pthread_mutex_unlock(&mutex);
		}
		else
		{
			
			pthread_cond_signal(&cond);
			pthread_mutex_unlock(&mutex);
			pthread_mutex_lock(&mutex2);
			pthread_cond_wait(&cond2, &mutex2);
			pthread_mutex_unlock(&mutex2);
		}
		
	}
}
void* child2(void* arg)
{

	while (1)
	{
		pthread_mutex_lock(&mutex);
		pthread_cond_wait(&cond, &mutex);       //获得信号之前，会重新获得互斥锁

		printf("我是线程  2  打印5的倍数:  %d \n", i);
		pthread_mutex_unlock(&mutex);
		//pthread_mutex_lock(&mutex2);
		pthread_mutex_lock(&mutex2);
		pthread_cond_signal(&cond2);
		pthread_mutex_unlock(&mutex2);
		//pthread_mutex_lock(&mutex2);
		  //需要在此处释放互斥锁

	}
}

int main(int argc, char* argv[])
{
	/*pthread_t tcp_recevie_id;
	pthread_t tcp_send_id;
	pthread_t udp_recevie_id;
	pthread_t udp_send_id;

	int ret = pthread_create(&tcp_recevie_id, NULL, recevie, NULL);
	if (ret != 0)
	{
			cout << "tcp_recevie_create error: error_code=" << ret << endl;
	}
	ret = pthread_create(&tcp_send_id, NULL, send, NULL);
	if (ret != 0)
	{
		cout << "tcp_send_create error: error_code=" << ret << endl;
	}
	ret = pthread_create(&tcp_send_id, NULL, send, NULL);
	if (ret != 0)
	{
		cout << "tcp_send_create error: error_code=" << ret << endl;
	}


	pthread_exit(NULL);*/
	//pthread_mutex_init(&mu1, NULL);

	pthread_t id;
	pthread_t idd;
	
	/*pthread_create(&idd, NULL, a, NULL);
	pthread_create(&id, NULL, b, NULL);

	pthread_join(id, NULL);
	pthread_join(idd, NULL);

	pthread_mutex_destroy(&mu1);*/

	pthread_cond_init(&cond, NULL);
	pthread_cond_init(&cond2, NULL);
	pthread_mutex_init(&mutex, NULL);
	pthread_mutex_init(&mutex2, NULL);

	pthread_create(&id, NULL, (void* (*)(void*))child1, NULL);
	pthread_create(&idd, NULL, child2, NULL);

	pthread_exit(NULL);
	return 0;
}