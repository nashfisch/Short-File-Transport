#include <iostream>
#include <iomanip>
#include <vector>
#include "Sender.h"
#include "SlidingWindow.h"
#include "SimpleHeader.h"
#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <zlib.h>
/*
Sender::Sender(int max) : SlidingWindow(max){}
int Sender::retransmit(){
    return unacknowledgedNums[0];
}*/

Sender::Sender(std::string& destinationHost, std::string& destinationPort, int max) : SlidingWindow(max) {
    destHost = destinationHost;
    destPort = destinationPort;
    Initialize();
}

void Sender::Initialize() {
    struct addrinfo hints;
    memset(&hints, 0 , sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;

    int rval = getaddrinfo(destHost.c_str(), destPort.c_str(), &hints, &destAddr);
    if (rval != 0) {
        std::cerr << "Error getting the destination address: " << gai_strerror(rval) << std::endl;
        exit(1);
    }

    sock = socket(destAddr->ai_family, destAddr->ai_socktype, destAddr->ai_protocol);
    if(sock == -1) {
        std::cerr << "Failed to open socket" << std::endl;
        exit(1);
    }
}

void Sender::SendMessage(char* message, int packetLength, SimpleHeader* header) {

    if (sendto(sock, message, packetLength, 0, destAddr->ai_addr, destAddr->ai_addrlen) == -1) {
        std::cerr << "Error sending the message" << std::endl;
        exit(1);
    }
    u_int8_t* ptr = nullptr;
    u_int8_t bounceback[524];
    socklen_t clientAddressLength;
    struct sockaddr_storage fromaddr;
    clientAddressLength = sizeof(fromaddr);
    
    int receivedBytes = recvfrom(sock, bounceback, sizeof(bounceback)-1, 0, (struct sockaddr*)&fromaddr, &clientAddressLength);

    if (receivedBytes == -1) {
        std::cerr << "Error receiving data" << std::endl;
    }

    //bounceback[receivedBytes] = 0;
    if ((bounceback[0] >> 6) == 2){
        ptr = header->getBufferAddress();
        header->setType(1);
        header->setTR(0);
        header->setSeqNum(1);
        header->setWindow(0);
        header->setTimeStamp(0);
        int payloadLength = header->getPayloadLength();
        header->setPayloadLength(0);
        for (int i = 12; i < payloadLength + 4; i++){
            ptr[i] = 0x00;
        }
        int EOTCRC = crc32(0L, NULL, 0);
        EOTCRC = crc32(EOTCRC, reinterpret_cast<const Bytef*>(ptr), 8);
        header->setCRC1(EOTCRC);
        
        sendto(sock, ptr, 12, 0, destAddr->ai_addr, destAddr->ai_addrlen);
    }
    ptr = nullptr;
    receivedBytes = recvfrom(sock, bounceback, sizeof(bounceback)-1, 0, (struct sockaddr*)&fromaddr, &clientAddressLength);
    close(sock);
}
