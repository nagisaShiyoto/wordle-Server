#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <thread>
#pragma comment(lib, "Ws2_32.lib")
#pragma warning(disable : 4996)
////////////////////include winsock2///////////////////

#include <string>
#include <vector>
#include <windows.h>
#include <iostream>
#define PORT 6969
#define IP "127.0.0.1"
#define LEN_SPACE 5

char* charLen(std::string str);
int sendName(SOCKET cs);
int manageUser(SOCKET cs);

int main()
{

	{
		int iResult;
		WSADATA wsaData;
		//MAKEWORD=the version of microsoft(2.2)
		iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (iResult != 0) {
			std::cout << "WSAStartup failed: " << iResult << std::endl;
			return 1;
		}
	}

	/////////////////////////create socket//////////////////////////////////
	SOCKET comu = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	sockaddr_in Sdata = { 0 };
	Sdata.sin_addr.s_addr=inet_addr(IP);//127.0.0.1
	Sdata.sin_port = htons(PORT);//6969
	Sdata.sin_family = AF_INET;//tcp
	if (connect(comu, (const sockaddr*)&Sdata, sizeof(Sdata)) == SOCKET_ERROR)
	{
		int error = GetLastError();
		if (error == 10061)
		{
			std::cout << "server offline";
			return error;
		}
		std::cout << "error: " << error;
		return error;
	}
	/////////////////////////create socket//////////////////////////////////

	////////////////////////////communicate with server//////////////////////////////////
	if (sendName(comu) == 1)
	{
		return 1;
	}
	manageUser(comu);
	////////////////////////////communicate with server//////////////////////////////////
	return 0;

}

char* charLen(std::string str)
{
	char* len=new char[LEN_SPACE];
	int size = str.length();
	int i = 0;
	if (size > 9999)
	{
		throw "to big of a name";
	}
	while (size > 0)
	{
		len[LEN_SPACE-i-1] = (size % 10)+'0';
		i++;
		size = size / 10;
	}
	for (int j = 0; j <LEN_SPACE-i ; j++)
	{
		len[j] = '0';
	}
	len[LEN_SPACE] = 0;
	return len;
}

int sendName(SOCKET cs)
{

	std::string toSend = "";
	char* len = { 0 };
	char con[2] = { 0 };
	std::cout << "enter your name:"<<std::endl;
	std::cin >> toSend;
	len = charLen(toSend);

	send(cs, len, LEN_SPACE, 0);//send len
	send(cs, toSend.c_str()+0, sizeof(toSend)+1, 0);//send massage
	recv(cs, con, 2, 0);//get authircation that got one:)
	if (con[0] == 'e')
	{
		std::cout << "error in opening file,try moving the file 'word.txt'";
		return 1;
	}
	return 0;
}

int manageUser(SOCKET cs)
{
	char status[5] = { 'o','k',0,0,0 };
	//////////////////send gues/////////////////////
	std::string gues;
	char* len = { 0 };
	std::cout << "what is your guess:\n";
	std::cin >> gues;
	len = charLen(gues);
	send(cs, len, LEN_SPACE, 0);
	send(cs, gues.c_str(), gues.length(), 0);
	//////////////////send gues/////////////////////

	/////////////////get responce///////////////////
	int size = 0;
	len[0] = 0;
	while (!(len[0] >= '0' && len[0] <= '9'))
	{
		recv(cs, len, LEN_SPACE, 0);
	}
	size = atoi(len)+1;
	char* msg = new char[size];
	recv(cs, msg, size, 0);
	std::string yourmam="";
	for(int i=0;i<size;i++)
	{
		if (msg[i] != 0)
		{
			yourmam += msg[i];
		}
	}
	std::cout <<yourmam;
	//send it got it
	send(cs, status, LEN_SPACE, 0);
	/////////////////get responce///////////////////
	return 0;
}