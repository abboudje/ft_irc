#include "Client.hpp"

Client::Client(){
    this->_fd = -1;
    this->_isAuth = false;
}

Client::Client(int fd, const struct sockaddr_in& addr){

    this->_fd = fd;
    this->_isAuth = false;
    this->_clientAddr = addr;
}

Client::~Client(){}

Client::Client(const Client& other){
    *this = other;
}

Client& Client::operator=(const Client& other){
    if (this != &other)
    {
        _fd = other._fd;
        _buffer = other._buffer;
        _nickname = other._nickname;
        _username = other._username;
        _clientAddr = other._clientAddr;
        _isAuth = other._isAuth;
    }
    return (*this);
}

int Client::getFd() const { return _fd; }

std::string& Client::getBuffer() { return _buffer; }

const std::string& Client::getNickname() const { return _nickname; }

const std::string& Client::getUsername() const { return _username; }

void Client::setNickname(const std::string& nickname) { _nickname = nickname; }

void Client::setUsername(const std::string& username) { _username = username; }

void Client::setAuthenticated(bool authenticated) { _isAuth = authenticated; }

bool Client::isAuthenticated() const { return _isAuth; }