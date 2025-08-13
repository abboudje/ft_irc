#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include <iostream>
#include <cstdlib>
#include <csignal>

Server* g_server = NULL;
void signalHandler(int signum) {
    (void)signum;
    std::cout << "\nSignal received, shutting down the server." << std::endl;
    if (g_server) {
        delete g_server;
        g_server = NULL;
    }
    exit(0);
}
int main(int argc, char *argv[]) {
    if (argc != 3){
        std::cerr << "Usage: " << argv[0] <<" <port> <password>" << std::endl; 
        return (1);
    }
    try
    {
        int port = std::atoi(argv[1]);
        std::string password = argv[2];
        if (port < 1024 || port > 65535){
            throw std::invalid_argument("Error: Port number must be between 1024 and 65535.");
        }
        signal(SIGINT, signalHandler);
        signal(SIGQUIT, signalHandler);
        g_server = new Server(port, password);
        g_server->run();
        delete g_server;
        g_server  = NULL;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        if (g_server){
            delete g_server;
            g_server = NULL;
        }
        return (1);
    }
    return 0;
}
