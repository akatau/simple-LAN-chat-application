#include <iostream>
#include <string>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <algorithm>

class ChatServer {
private:
    int serverSocket; // The server socket file descriptor
    std::vector<int> clientSockets; // A vector to store the file descriptors of connected clients

public:
    /**
     * Constructor to create a ChatServer instance
     * @param port The port number to listen on
     */
    ChatServer(int port);

    /**
     * Start the chat server and begin listening for incoming connections
     */
    void start();

    /**
     * Broadcast a message to all connected clients
     * @param message The message to broadcast
     */
    void broadcastMessage(const std::string& message);

    /**
     * Handle incoming messages from a single client
     * @param clientSocket The file descriptor of the client socket
     */
    void handleClient(int clientSocket);
};

ChatServer::ChatServer(int port) {
    // Create a socket using the AF_INET address family (IPv4) and SOCK_STREAM type (TCP)
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Could not create socket" << std::endl;
        exit(1);
    }

    // Set up the server address structure
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port); // Convert port number to network byte order
    serverAddr.sin_addr.s_addr = INADDR_ANY; // Listen on all available network interfaces

    // Bind the socket to the specified address and port
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Bind failed" << std::endl;
        exit(1);
    }

    // Listen for incoming connections with a backlog of 3
    if (listen(serverSocket, 3) < 0) {
        std::cerr << "Listen failed" << std::endl;
        exit(1);
    }
}

void ChatServer::start() {
    while (true) {
        // Accept an incoming connection
        sockaddr_in clientAddr;
        socklen_t addr_size = sizeof(clientAddr);
        int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &addr_size);
        if (clientSocket < 0) {
            std::cerr << "Accept failed" << std::endl;
            continue;
        }

        // Add the client socket to the vector of connected clients
        clientSockets.push_back(clientSocket);

        // Create a new thread to handle the client
        std::thread clientThread(&ChatServer::handleClient, this, clientSocket);
        clientThread.detach(); // Detach the thread to allow it to run concurrently
    }
}

void ChatServer::handleClient(int clientSocket) {
    char buffer[1024] = {0}; // Buffer to store incoming messages

    while (true) {
        // Read incoming data from the client
        int valRead = read(clientSocket, buffer, 1024);
        if (valRead <= 0) {
            // Client disconnected
            close(clientSocket);
            clientSockets.erase(std::remove(clientSockets.begin(), clientSockets.end(), clientSocket), clientSockets.end());
            break;
        }

        // Convert the buffer to a string and broadcast it to all clients
        std::string message(buffer, valRead);
        broadcastMessage(message);
    }
}

void ChatServer::broadcastMessage(const std::string& message) {
    // Iterate over the vector of connected clients and send the message to each one
    for (int clientSocket : clientSockets) {
        send(clientSocket, message.c_str(), message.size(), 0);
    }
}

int main() {
    // Create a ChatServer instance and start it on port 8081
    ChatServer server(8081);
    server.start();
    return 0;
}