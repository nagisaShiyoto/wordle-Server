#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#pragma comment(lib, "Ws2_32.lib")
////////////////////include winsock2///////////////////
#include <iostream>
#include <Windows.h>	
#include <fstream>
#include <cstdlib>
#include <string>
#define WORD_COUNT 12972
class User
{
private:
	int _tries;
	std::string _UserName;
	std::string _SecreteWord;
	char* _SWC;
	SOCKET _userSocket;
	std::string getRandWord(std::string path);
	
public:
	
	//constractor
	User(std::string userName, SOCKET sock, std::string path = "words.txt");

	//getters
	std::string getUserName()const;
	std::string getWord()const;
	char* getSWC()const;
	SOCKET getSocket()const;
	int getTries()const;

	//setters
	char* charLen(std::string str);
	void setUserName(std::string const userName);
	void setWord(std::string const path);
	void addTry();
	void manageMsg(int amount_of_tries);
	std::string createResponse(std::string userMsg,char* charMsg,int msgL, int amount_of_tries);
};

