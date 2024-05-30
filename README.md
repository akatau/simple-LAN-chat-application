# Simple LAN Chat Application

This repository contains the source code for a simple LAN chat application implemented in C++. The application consists of two main components: a client and a server. Both components communicate over a local area network (LAN) using TCP/IP protocols. This setup allows users to send and receive messages in real-time across devices connected to the same network.

## Overview

The chat application is designed to facilitate communication between users within a local network. It leverages the power of sockets for establishing connections and transferring data. The server component listens for incoming connections and broadcasts messages to all connected clients, while the client component sends messages to the server and receives messages from other clients through the server.

### Key Features

- **Real-Time Communication**: Users can send and receive messages instantly across the network.
- **Multi-threading**: Utilizes threading for handling multiple clients simultaneously on the server side and for separating message reception from message transmission on the client side.
- **Cross-platform Compatibility**: Written in C++, ensuring compatibility across various operating systems that support C++.

## Getting Started

To get started with the chat application, follow these steps:

1. **Clone the Repository**: Clone this repository to your local machine using Git.

2. **Build the Project**: Use a C++ compiler compatible with the version used for development. Compile both the client and server components separately.

3. **Run the Server**: Before starting the client, ensure the server is running. Run the server executable from the command line.

4. **Run the Client**: Open another terminal window and run the client executable. You will need to specify the server's IP address and the port number when prompted.

5. **Start Communicating**: Once both the server and client are running, you can start sending and receiving messages.

## Code Structure

The project is divided into two main classes: `ChatClient` and `ChatServer`.

- **ChatClient**: Handles the client-side logic, including connecting to the server, sending messages, and receiving messages in a separate thread.
- **ChatServer**: Manages the server-side operations, such as accepting incoming connections, broadcasting messages to all connected clients, and handling individual client connections in separate threads.

## Dependencies

- **C++ Compiler**: GCC or Clang.
- **Standard Libraries**: `<iostream>`, `<string>`, `<vector>`, `<sys/socket.h>`, `<netinet/in.h>`, `<arpa/inet.h>`, `<unistd.h>`, `<thread>`.

## License

This project is licensed under the MIT License. See the LICENSE file for more details.