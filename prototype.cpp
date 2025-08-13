#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <cerrno>

int main() {

    // Create a socket and bind it to port 6667
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {    
        std::cerr << "Error creating socket: " << strerror(errno) << std::endl;
        return 1;
    }
    // Set up the server address structure
    struct sockaddr_in server_addr;
    std::memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(6667);
    // Bind the socket to the address and port
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Error binding socket: " << strerror(errno) << std::endl;
        close(server_fd);
        return 1;
    }

    // Listen for incoming connections
    if (listen(server_fd, 5) < 0) {
        std::cerr << "Error listening on socket: " << strerror(errno) << std::endl;
        close(server_fd);
        return 1;
    }

    while (true)
    {
        int client_fd;
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
        if (client_fd < 0){
            std::cerr << "Error accepting connection: " << strerror(errno) << std::endl;
            //close(server_fd);
            return (1);
        }
        std::cout << "Client connected!" << std::endl;
        char buffer[512];
        ssize_t bytes_received = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received > 0){
            buffer[bytes_received] = '\0';
            std::cout << "Received: " << buffer << std::endl;
            send(client_fd, buffer, bytes_received, 0);
        }
        close(client_fd);
    }
    // get data 

    // Successfully created and bound the socket
    std::cout << "Server is listening on port 6667." << std::endl;
    std::cout << "Socket created and bound successfully." << std::endl;
    close(server_fd);
    std::cout << "Socket closed." << std::endl;
    return 0;
}
