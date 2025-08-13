// Channel.hpp

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <vector>
#include <map>
#include <algorithm> // For std::find
#include <sys/socket.h> // for send()
#include <unistd.h> // for write()
#include "Client.hpp"

class Client;

class Channel {
    private:
        typedef std::vector<Client*>::iterator client_iterator;
    
        std::string             _name;
        std::string             _topic;
        std::vector<Client*>    _clients;
        std::vector<int>        _operatorFds; // Store FDs of operators
        Channel(); // Default constructor disabled
        
        // Channel Modes (to be implemented)
        // bool _inviteOnly;
        // std::string _key;
        // int _userLimit;
    public:
        // Constructor & Destructor
        Channel(const std::string& name);
        ~Channel();

        // Member Management
        void addClient(Client* client);
        void removeClient(Client* client);
        void addOperator(Client* client);
        void removeOperator(Client* client);
        bool isOperator(int clientFd);

        // Getters & Setters
        const std::string& getName() const;
        const std::string& getTopic() const;
        void setTopic(const std::string& topic);

        // Broadcast message to all clients in the channel
        void broadcast(const std::string& message, int senderFd);
};

#endif