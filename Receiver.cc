#include <iostream>
#include <fstream>
#include <vector>
#include "Receiver.h"
#include "SlidingWindow.h"
#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>

/*
Receiver::Receiver(int max) : SlidingWindow(max){
    setMax(max);
    setWinSize(1);
}*/

Receiver::Receiver(std::string& listenPort, std::string& IPvType, int max) : SlidingWindow(max) {
    lPort = listenPort;
    Initialize();
    if(IPvType != "-4" || IPvType != "-6") {
        IPv = "-4";
    }
    IPv = IPvType;
}

void Receiver::Initialize() {
    if (IPv == "-4") {
        sock = socket(AF_INET, SOCK_DGRAM, 0);
    } else if (IPv == "-6") {
        sock = socket(AF_INET6, SOCK_DGRAM, 0);
    }
    if (sock == -1) {
        std::cerr << "Error creating socket" << std::endl;
        exit(1);
    }
    
    // The receiver's address
    sockaddr_in recAddr;
    // The kind of socket used by the receiver
    if (IPv == "-4") {
        recAddr.sin_family = AF_INET;
    } else if (IPv == "-6") {
        recAddr.sin_family = AF_INET6;
    }
    // Converts our port number initialized in main into network usable data
    recAddr.sin_port = htons(std::stoi(lPort));
    // Address of socket that we will bind to
    recAddr.sin_addr.s_addr = INADDR_ANY;
    

    if(bind(sock, (struct sockaddr*)&recAddr, sizeof(recAddr)) == -1) {
        std::cerr << "Error binding socket" << std::endl;
        close(sock);
        exit(1);
    }
}


//originally std::string, removed to get rid of the warning
void Receiver::ReceiveMessage() {
    char buffer[1024];
    // socklen_t clientAddressLength;
    struct sockaddr_storage fromAddr;
    fromAddrLength = sizeof(fromAddr);
    // Does this get header + payload or just payload?
    int receivedBytes = recvfrom(sock, buffer, sizeof(buffer)-1, 0, (struct sockaddr*)&fromAddr, &fromAddrLength);
    if (receivedBytes == -1) {
        std::cerr << "Error receiving data" << std::endl;
    }
    buffer[receivedBytes] = 0;
    std::cout << "Received: " << buffer << std::endl;

    // Sendto used for bounceback
    sendto(sock, buffer, sizeof(buffer) - 1, 0, (struct sockaddr*)&fromAddr, fromAddrLength);

    close(sock);
}

void Receiver::ReceiveFile(std::string fileName, std::ofstream& stream) {
    stream.open(fileName, std::ios::binary);
    char buffer[1024];
    // socklen_t clientAddressLength;
    struct sockaddr_storage fromAddr;
    fromAddrLength = sizeof(fromAddr);
    // Does this get header + payload or just payload?
    int receivedBytes = recvfrom(sock, buffer, sizeof(buffer)-1, 0, (struct sockaddr*)&fromAddr, &fromAddrLength);
    if (receivedBytes == -1) {
        std::cerr << "Error receiving data" << std::endl;
    }
    buffer[receivedBytes] = 0;
    for (int i = 0; i < receivedBytes - 4; i++){
        stream << buffer[i + 12];
    }
    
    // Set bounceback packet type to 2 (ACK)
    buffer[0] &= 0x3f;
    buffer[0] |= (2 << 6); 
    // Sendto used for bounceback
    sendto(sock, buffer, sizeof(buffer) - 1, 0, (struct sockaddr*)&fromAddr, fromAddrLength);

    close(sock);
}


