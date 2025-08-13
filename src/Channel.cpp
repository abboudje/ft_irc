#include "Channel.hpp"


Channel::Channel(const std::string& name) : _name(name) {}

Channel::~Channel() {}

void Channel::addClient(Client* client) {
    _clients.push_back(client);
}

void Channel::removeClient(Client* client) {
    for (client_iterator it = _clients.begin(); it != _clients.end(); ++it) {
        if ((*it)->getFd() == client->getFd()) {
            _clients.erase(it);
            break;
        }
    }
    // Also remove from operators if they were one
    removeOperator(client);
}

void Channel::addOperator(Client* client) {
    if (!isOperator(client->getFd())) {
        _operatorFds.push_back(client->getFd());
    }
}

void Channel::removeOperator(Client* client) {
    std::vector<int>::iterator it = std::find(_operatorFds.begin(), _operatorFds.end(), client->getFd());
    if (it != _operatorFds.end()) {
        _operatorFds.erase(it);
    }
}

bool Channel::isOperator(int clientFd) {
    return std::find(_operatorFds.begin(), _operatorFds.end(), clientFd) != _operatorFds.end();
}

const std::string& Channel::getName() const {
    return _name;
}

const std::string& Channel::getTopic() const {
    return _topic;
}

void Channel::setTopic(const std::string& topic) {
    _topic = topic;
}

void Channel::broadcast(const std::string& message, int senderFd) {
    for (client_iterator it = _clients.begin(); it != _clients.end(); ++it) {
        if ((*it)->getFd() != senderFd) {
            // Add \r\n to conform to IRC protocol
            std::string full_message = message + "\r\n";
            if (send((*it)->getFd(), full_message.c_str(), full_message.length(), 0) < 0) {
                // Handle send error if necessary
            }
        }
    }
}