#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <poll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdexcept>
#include <cstring> // For strerror
#include <cerrno>  // For errno
#include <iostream>

class Client;
class Server{
    private:
        typedef std::vector<struct pollfd> PollFds;
        typedef std::map<int, Client> ClientMap;
        int         _port;
        int         _serverFd;
        std::string _password;
        PollFds     _pollFds;
        ClientMap   _clients;   
        void        _setupServer();
        void        _acceptNewClient();
        void        _handleClientData(int clientFd);
        void        _removeClient(int clientIdx);
        void        _processCommand(int clientFd, const std::string& command);
        Server();
        Server(const Server& other);
        Server& operator=(const Server& other);
        
    public:
        Server(int port, std::string& password);
        ~Server();
        void run();
};

#endif