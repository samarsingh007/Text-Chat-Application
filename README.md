# Text Chat Application

## Introduction
This Text Chat Application is a C++ project designed to facilitate real-time messaging between clients through a central server. The application showcases client-server architecture, with the server handling connections, and message relay, and the clients facilitating user interaction and message transmission.

## Architecture
- **Server:** Manages client connections, broadcasts messages, and supports functionalities like login, logout, and message forwarding.
- **Client:** Connects to the server, sends, and receives messages. It supports functionalities like sending private messages or broadcasting messages to all connected clients.

## Technology Stack
- C++ for core application development.
- POSIX sockets for network communication.
- Multithreading for handling multiple client connections concurrently (implicit in the networking logic).
- Standard C++ libraries (`<bits/stdc++.h>`, `<fcntl.h>`, etc.) for data manipulation and IO operations.

## Prerequisites
- Linux or Unix-like operating system (for POSIX socket support)
- GNU Compiler Collection (GCC) for compiling C++ code

## Compilation
Navigate to the project directory and compile the server and client code using the following commands:

For the server:
```bash
g++ -o server server.cpp -pthread
