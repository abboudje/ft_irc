#include "Server.hpp"
#include "Client.hpp"

Server::Server(){
}

Server::Server(int port, std::string& password){
    _port = port;
    _serverFd = -1;
    _password = password;
    std::cout << "Initializing server..." << std::endl;
    _setupServer();
}

// _setupServer: Creates, configures, binds, and listens on the server socket.
void Server::_setupServer(){
    // creation
    _serverFd = socket(AF_INET, SOCK_STREAM, 0);
    if (_serverFd < 0) {
        throw std::runtime_error("Error: Failed to create socket: " + std::string(strerror(errno)));
    }

    // setup 
    int opt = 1;
    if (setsockopt(_serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        throw std::runtime_error("Error: Failed to set socket options: " + std::string(strerror(errno)));
    }

    // set  to non-blocking
    if (fcntl(_serverFd, F_SETFL, O_NONBLOCK) < 0) {
        throw std::runtime_error("Error: Failed to set socket to non-blocking: " + std::string(strerror(errno)));
    }

    // bind socket
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(_port);
    if (bind(_serverFd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        throw std::runtime_error("Error: Failed to bind socket: " + std::string(strerror(errno)));
    }

    // listen for connections
    if (listen(_serverFd, 10) < 0) {
        throw std::runtime_error("Error: Failed to listen on socket: " + std::string(strerror(errno)));
    }

    // add server socket to the poll vector
    struct pollfd serverPollFd;
    serverPollFd.fd = _serverFd;
    serverPollFd.events = POLLIN;
    _pollFds.push_back(serverPollFd);

    std::cout << "Server setup complete. Listening on port " << _port << "." << std::endl;
}

//  acceptNewClient: Accepts a new client connection.
void Server::_acceptNewClient() {
    struct sockaddr_in clientAddr;
    socklen_t clientLen = sizeof(clientAddr);
    int clientFd = accept(_serverFd, (struct sockaddr*)&clientAddr, &clientLen);
    if (clientFd < 0) {
        std::cerr << "Warning: accept() failed: " << strerror(errno) << std::endl;
        return;
    }

    // set client socket to non-blocking
    fcntl(clientFd, F_SETFL, O_NONBLOCK);

    // Add new client to poll vector
    struct pollfd clientPollFd;
    clientPollFd.fd = clientFd;
    clientPollFd.events = POLLIN;
    _pollFds.push_back(clientPollFd);

    // Create and store new Client object
    _clients.insert(std::make_pair(clientFd, Client(clientFd, clientAddr)));

    std::cout << "New connection from " << inet_ntoa(clientAddr.sin_addr)
              << ":" << ntohs(clientAddr.sin_port) << " on fd " << clientFd << std::endl;
}


// _handleClientData: Reads and processes data from a client.
void Server::_handleClientData(int clientFd) {
    char buffer[1024];
    Client& client = _clients.at(clientFd);

    ssize_t bytesRead = recv(clientFd, buffer, sizeof(buffer) - 1, 0);

    if (bytesRead > 0) {
        buffer[bytesRead] = '\0';
        client.getBuffer().append(buffer);
        std::cout << "Received from fd " << clientFd << ": " << buffer;

        // Process complete commands from the buffer
        std::string& clientBuffer = client.getBuffer();
        size_t pos;
        while ((pos = clientBuffer.find("\r\n")) != std::string::npos) {
            std::string command = clientBuffer.substr(0, pos);
            clientBuffer.erase(0, pos + 2); // Remove command and \r\n
            
            // This is where you will call your command handler
            std::cout << "Processing command from fd " << clientFd << ": " << command << std::endl;
            // _processCommand(clientFd, command); // TODO: parsing et exec command ya Abdesami3
        }
    } else if (bytesRead == 0) {
        // disconnection
        std::cout << "Client on fd " << clientFd << " disconnected." << std::endl;
        // Find index to remove
        for (size_t i = 1; i < _pollFds.size(); ++i) {
            if (_pollFds[i].fd == clientFd) {
                _removeClient(i);
                break;
            }
        }
    } else {
        // An error occurred (recv < 0)
        if (errno != EWOULDBLOCK && errno != EAGAIN) {
            std::cerr << "Error reading from client fd " << clientFd << ": " << strerror(errno) << std::endl;
            // Find index to remove
            for (size_t i = 1; i < _pollFds.size(); ++i) {
                if (_pollFds[i].fd == clientFd) {
                    _removeClient(i);
                    break;
                }
            }
        }
    }
}

// _removeClient: Removes a client from the server.
void Server::_removeClient(int clientIdx) {
    int clientFd = _pollFds[clientIdx].fd;

    // Close the socket
    close(clientFd);

    // Remove from data structures
    _clients.erase(clientFd);
    _pollFds.erase(_pollFds.begin() + clientIdx);

    std::cout << "Client on fd " << clientFd << " has been removed." << std::endl;
}

void Server::run(){
    std::cout << "Server is running." << std::endl;
    while (true) {
        // Wait for an event on one of the sockets
        int pollCount = poll(&_pollFds[0], _pollFds.size(), -1);
        if (pollCount < 0) {
            throw std::runtime_error("Error: poll() failed: " + std::string(strerror(errno)));
        }

        // Check for new connection on the server socket
        if (_pollFds[0].revents & POLLIN) {
            _acceptNewClient();
        }

        // Check for data from clients
        for (size_t i = 1; i < _pollFds.size(); ++i) {
            if (_pollFds[i].revents & POLLIN) {
                _handleClientData(_pollFds[i].fd);
            }
             // Check if client disconnected or an error occurred
            if (_pollFds[i].revents & (POLLHUP | POLLERR)) {
                _removeClient(i);
            }
        }
    }
}

Server::~Server(){
    std::cout << "Shutting down server..." << std::endl;
    for (ClientMap::iterator it = _clients.begin(); it != _clients.end(); ++it) {
        close(it->first);
    }
    if (_serverFd != -1) {
        close(_serverFd);
    }
}
