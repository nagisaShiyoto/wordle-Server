#include "User.h"
#include <cstdlib>
#include <mutex>
#define ERROR_CODE "ERROR123"
#pragma warning(disable : 4996)
#define LEN_SPACE 5
std::mutex openF;
std::string User::getRandWord(std::string path)
{
    std::string word = "";
    openF.lock();
        std::ifstream file(path);
        if (!file.is_open())
        {
            std::cout << "wrong file path:)";
            throw ERROR_CODE;
        }
        srand(time(0));
        int lineP = std::rand() % WORD_COUNT;
        for (int i = 0; i < lineP; i++)
        {

            std::getline(file, word);
        }
        file.close();
    openF.unlock();
    return word;
}

User::User(std::string userName, SOCKET sock, std::string path)
{
    _tries = 0;
    this->setWord(path);
    this->setUserName(userName);
    this->_userSocket = sock;
}

std::string User::getUserName() const
{
    return this->_UserName;
}

std::string User::getWord() const
{
    return this->_SecreteWord;
}

char* User::getSWC() const
{
    return this->_SWC;
}

SOCKET User::getSocket() const
{
    return this->_userSocket;
}

int User::getTries() const
{
    return this->_tries;
}

char* User::charLen(std::string str)
{
    char* len = new char[LEN_SPACE];
    int size = str.length();
    int i = 0;
    if (size > 9999)
    {
        throw "to big of a name";
    }
    while (size > 0)
    {
        len[LEN_SPACE - i - 1] = (size % 10) + '0';
        i++;
        size = size / 10;
    }
    for (int j = 0; j < LEN_SPACE - i; j++)
    {
        len[j] = '0';
    }
    len[LEN_SPACE] = 0;
    return len;
}

void User::setUserName(std::string userName)
{
    this->_UserName = userName;
}

void User::setWord(std::string path)
{
    this->_SecreteWord = this->getRandWord(path);
    //strdup=convert const char* to char*
    this->_SWC =_strdup( this->_SecreteWord.c_str());
}

void User::addTry()
{
    this->_tries++;
}

void User::play(int amount_of_tries)
{
    std::string res = "";
    char con[2]={'c',0};
    while (con[0] = 'c')
    {
        this->setWord();
        res = "";
        this->_tries = 0;
        while (res!="out of tries try again later"&&res!="you got the word")
        {
            res=this->manageMsg(amount_of_tries);
        }
        send(this->getSocket(), this->charLen(this->getWord()), 5, 0);
        send(this->getSocket(), this->getSWC(), this->getWord().length(), 0);
        recv(this->getSocket(), con, 2, 0);
    }
}

std::string User::manageMsg(int amount_of_tries)
{
    ////////////////////get msg/////////////////////////////
    int size = 0;
    char* len = new char[5];
    len[0] = 0;
    while (!(len[0]>='0'&&len[0]<='9'))
    {
        recv(this->getSocket(), len, 5, 0);
    }
    size = atoi(len);

    char* name = new char[size];
    recv(this->getSocket(), name, size, 0);
    name[size] = 0;
    std::string nm = name;
    ////////////////////get msg/////////////////////////////
    ////////////////////game////////////////////////////////
    this->addTry();
    std::string responce =this->createResponse(nm, name, size,amount_of_tries);
    ////////////////////game////////////////////////////////
    ///////////////////send responce///////////////////////

    len = this->charLen(responce);  
    send(this->getSocket(), len, LEN_SPACE, 0);
    send(this->getSocket(), responce.c_str()+0, responce.length()+1, 0);
    recv(this->getSocket(), len, LEN_SPACE, 0);
    //////////////////send responce////////////////////////
    return responce;
}



std::string User::createResponse(std::string userMsg, char* charMsg,int msgL,int amount_of_tries)
{
    int rightCounter = 0;
    std::string msg = "spotOn: ";
    std::string close = "";
    for (int i = 0; i < this->getWord().length(); i++)
    {
        if (this->getSWC()[i] == charMsg[i])
        {
            msg += charMsg[i];
            msg += ' ';
            charMsg[i] = 0;
            rightCounter++;
        }
        else
        {
            msg += "_ ";
        }
        for (int j = 0; j < this->getWord().length(); j++)
        {
            if (this->getSWC()[j] == charMsg[i] && i != j)
            {
                close += charMsg[i];
                charMsg[i] = 0;
            }
        }
    }
    //chek if got right:)
    if (rightCounter == this->getWord().length())
    {
        return "you got the word";
    }
    if (amount_of_tries <= this->getTries())
    {
        return "out of tries try again later";
    }
    msg += "\nnotPlace: ";
    msg += close+"\nthereIsnt: ";
    for (int i = 0; i < msgL; i++)
    {
        msg += charMsg[i];
    }
    msg += "\namount of tries: ";
    msg += this->getTries()+'0';
    std::cout << msg;
    return msg;
}
