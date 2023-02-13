#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <thread>
#pragma comment(lib, "Ws2_32.lib")
////////////////////include winsock2///////////////////
#include "User.h"
#include <vector>
#include <windows.h>
#include <mutex>
#define PORT 6969
#define IP 127.0.0.1
#define CONTINUE 'c'
#define ERROR_CODE "ERROR123"

void getName(SOCKET sr);

int main()
{
	int iResult;
	WSADATA wsaData;
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		std::cout << "WSAStartup failed: " << iResult << std::endl;
		return 1;
	}


	std::vector<std::thread> threads;
	std::string name = "";
	SOCKET ln= socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	sockaddr_in ipInfo;
	int len = 0;
	ipInfo.sin_family = AF_INET;//tcp
	ipInfo.sin_port = htons (PORT);//port=6969
	ipInfo.sin_addr.s_addr =INADDR_ANY;//get any ip
	if( bind(ln,(const sockaddr*)&ipInfo, sizeof(ipInfo)) ==SOCKET_ERROR)
	{
		std::cout << GetLastError();
		std::cout << "server socket didnt worked";
		return 1;
	}

	if (listen(ln, SOMAXCONN) == SOCKET_ERROR)
	{
		std::cout << "times out:(";
		return 1;
	}
	while (true)
	{
		len = (int)sizeof(ipInfo);

		SOCKET cl=accept(ln, NULL, NULL);
		//didnt checked
		threads.push_back(std::thread(getName, std::ref(cl)));
		
	}


	for (auto it = threads.begin(); it != threads.end(); it++)
	{
		it->join();
	}
	return 0;
}

void getName(SOCKET sr)
{
	int size = 0;
	////////////////////get len//////////////////////////
	char* len = new char[5];
	if(recv(sr, len, 5, 0)==SOCKET_ERROR)
		std::cout<<GetLastError();
	size=atoi(len)+1;
	////////////////////get len//////////////////////////
	///////////////////get name//////////////////////////
	char* name = new char[size];
	recv(sr, name, size, NULL);
	std::string nm = name;
	///////////////////get name//////////////////////////
	//////////sends got name and created user////////////
	try
	{
		char con[2] = { CONTINUE, 0 };
		User user(nm, sr);
		std::cout << "user connected: " + user.getUserName() + "\n";
		send(sr, con, 2, 0);
	//////////sends got name and created user////////////
		user.manageMsg(5);
	}
	catch (const char* msg)
	{
		char con[2] = { 'e',0 };
		send(sr, con, 2, 0);
	}
	
	
	
}
