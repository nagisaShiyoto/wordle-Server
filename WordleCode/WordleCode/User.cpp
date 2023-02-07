#include "User.h"
std::string User::getRandWord(std::string path)
{
    std::string word = "";
    std::ifstream file(path);
    if (!file.is_open())
    {
        std::cout << "wrong file path:)";
    }
    int lineP = rand() % WORD_COUNT;
    for (int i = 0; i < lineP; i++)
    {

        std::getline(file, word);
    }
    file.close();
    return word;
}

User::User(std::string userName, SOCKET sock, std::string path)
{
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

void User::setUserName(std::string userName)
{
    this->_UserName = userName;
}

void User::setWord(std::string path)
{
    this->_SecreteWord=this->getRandWord(path);
    //strdup=convert const char* to char*
    this->_SWC =_strdup( this->_SecreteWord.c_str());
}
