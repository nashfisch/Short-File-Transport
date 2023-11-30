#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include "Receiver.h"
#include "SlidingWindow.h"
#include "SimpleHeader.h"
#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <zlib.h>

#define BUFFER_SIZE 528
#define PAYLOAD_MAX 512

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
    char buffer[BUFFER_SIZE];
    char buffer2[12];
    char* betterStuff = nullptr;
    char* ptr = nullptr;
    SimpleHeader* myHeader = new SimpleHeader();

    unsigned long crc1;
    unsigned long crc2;
    
    // socklen_t clientAddressLength;
    struct sockaddr_storage fromAddr;
    fromAddrLength = sizeof(fromAddr);
    // Does this get header + payload or just payload?
    int receivedBytes = recvfrom(sock, buffer, sizeof(buffer)-1, 0, (struct sockaddr*)&fromAddr, &fromAddrLength);
    int payloadlen = receivedBytes - 16;
    //int timestamp = 
    myHeader->setBuffer(buffer, BUFFER_SIZE);
    myHeader->setType(2); 
    myHeader->setTR(0);
    myHeader->setWindow(31);
    myHeader->setSeqNum(1); // header[1]
    //myHeader->setTimeStamp(0);
    myHeader->setPayloadLength(0);

    crc1 = crc32(0L, NULL, 0);
    crc1 = crc32(crc1, reinterpret_cast<const Bytef*>(buffer), 8);
    myHeader->setCRC1(crc1);

    

    if (receivedBytes == -1) {
        std::cerr << "Error receiving data" << std::endl;
    }
    buffer[receivedBytes] = 0;

    for (int i = 0; i < receivedBytes - 16; i++){
        stream << buffer[i + 12];
    }
    
    for (int i = 0; i < 12; i++){
        buffer2[i] = buffer[i];
    }
    

    //betterStuff = &buffer2[12];

    /*
    crc2 = crc32(0L, NULL, 0);
    crc2 = crc32(crc2, reinterpret_cast<const Bytef*>(betterStuff), receivedBytes - 16);
    
    myHeader->setCRC2(crc2, receivedBytes - 16);
    */

    // Sendto used for bounceback
    sendto(sock, buffer2, sizeof(buffer2), 0, (struct sockaddr*)&fromAddr, fromAddrLength);

    close(sock);
}


