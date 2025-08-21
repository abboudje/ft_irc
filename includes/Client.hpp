#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>

class Client{
    private:
        int         _fd;
        std::string _buffer;
        std::string _nickname;
        std::string _username;
        bool        _isAuth;
        struct sockaddr_in  _clientAddr;
    public:
        Client();
        Client(int fd, const struct sockaddr_in& addr);
        Client(const Client& other);
        Client& operator=(const Client& other);
        ~Client();
        //getter
        int getFd() const;
        std::string& getBuffer();
        const std::string& getNickname() const;
        const std::string& getUsername() const;
        //setter
        void setNickname(const std::string& nickname);
        void setUsername(const std::string& username);
        void setAuthenticated(bool authenticated);
        //others
        bool isAuthenticated() const;
};
#endif