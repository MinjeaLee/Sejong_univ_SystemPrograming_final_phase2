# Advanced Network and Message Queue Calculation Server

## Overview
- This project is an advanced version of a network-based calculation server, now incorporating message queue (MQ) for inter-process communication (IPC). 
- It maintains the ability to handle multiple client requests simultaneously, leveraging both TCP/IP sockets and message queues. 
- This hybrid approach enhances the server's capability to manage client requests and process calculations asynchronously.

## Author Information
- **Author:** Lee Minjae
- **ID:** 21011741
- **Department:** Information Security
- **Date:** 2023.12.17
- **Contact:** leejoy2@sju.ac.kr

## Installation
To set up the server and client programs, follow these steps:
1. Clone the repository to your local machine.
2. Compile the code using the provided Makefile:
```bash
make all
```

## Usage
1. Start the server by running: 
   ```bash
   ./server
   ```
2. In separate terminal windows, start one or more clients by running:
   ```bash
   ./client_add
   ```
   or
   ```bash
   ./client_mul
   ```
   depending on the desired operation (addition or multiplication).

3. Follow the on-screen prompts on each client to input numbers and receive the calculation results from the server.

## Features
- Integrated Message Queue: The server and clients utilize message queues for efficient and asynchronous request handling.
- Concurrent Client Handling: The server continues to handle each client request in a separate thread, allowing for parallel processing.
- Hybrid Communication Mechanism: Utilizes both TCP/IP sockets and message queues, combining the reliability of sockets with the non-blocking advantages of message queues.
- Extended Operations: Supports addition and multiplication operations, catering to different client requests.
- Scalable Architecture: Designed to be scalable, accommodating an increasing number of client requests without significant performance degradation.
- User-Friendly Client Interaction: Clients interact with the server using simple command-line interfaces, making it accessible for users with varying levels of technical expertise.

## License
This project is open-source and available for modification and distribution under standard open-source licenses.