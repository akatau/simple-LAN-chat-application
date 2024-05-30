#include <iostream> // For standard input/output operations
#include <string>   // For string manipulation
#include <sys/socket.h>  // For socket creation and manipulation functions
#include <arpa/inet.h>  // For internet protocol address conversion functions
#include <unistd.h>    // For close, read, write, etc. system calls
#include <thread>     // For creating and managing threads

class ChatClient {
private:
    int clientSocket; // File descriptor for the client socket
    std::string serverIP;  // IP address of the chat server
    int serverPort;      // Port number used by the chat server

public:
    ChatClient(const std::string& ip, int port); // Constructor to initialize client
    void start();                                 // Starts the chat client loop
    void sendMessage(const std::string& message); // Sends a message to the server
    void receiveMessages();                       // Receives messages from the server in a separate thread
};

// Constructor: Initializes the client socket and connection details
ChatClient::ChatClient(const std::string& ip, int port) : serverIP(ip), serverPort(port) {
    // Create a socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        std::cerr << "Could not create socket" << std::endl;
        exit(1);
    }

    // Structure to hold server address information
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET; // Address family: Internet Protocol (IPv4)
    serverAddr.sin_port = htons(serverPort); // Convert port number to network byte order
    serverAddr.sin_addr.s_addr = inet_addr(serverIP.c_str()); // Convert IP address string to network byte order

    // Connect to the server
    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Connection failed" << std::endl;
        exit(1);
    }
}

// Starts the chat client by creating a thread for receiving messages and entering a loop for sending messages
void ChatClient::start() {
    // Create a thread to handle receiving messages from the server
    std::thread receiveThread(&ChatClient::receiveMessages, this);

    // Detach the thread so it runs independently from the main thread
    receiveThread.detach();

    // Main loop for sending messages to the server
    while (true) {
        std::string message;
        std::getline(std::cin, message);
        sendMessage(message);
    }
}

// Sends a message to the server
void ChatClient::sendMessage(const std::string& message) {
    // Send the message string to the server socket
    send(clientSocket, message.c_str(), message.size(), 0);
}

// Continuously receives messages from the server in a separate thread
void ChatClient::receiveMessages() {
    char buffer[1024] = {0}; // Buffer to store received messages
    while (true) {
        // Read data from the server socket
        int valRead = read(clientSocket, buffer, 1024);

        // Check for errors or connection termination
        if (valRead <= 0) {
            std::cerr << "Server disconnected" << std::endl;
            break;
        }

        // Create a string object from the received data
        std::string message(buffer, valRead);

        // Print the received message to the console
        std::cout << "Received message: " << message << std::endl;
    }
}

int main() {
    // Create a ChatClient object with server IP and port
    ChatClient client("127.0.0.1", 8081);

    // Start the chat client
    client.start();

    return 0;
}
