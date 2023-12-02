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
#define ACK_SIZE 12
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
    char buffer[BUFFER_SIZE]; //receiving buffer
    char buffer2[ACK_SIZE]; //acknowledgement buffer

    SimpleHeader* myHeader = new SimpleHeader();

    unsigned long crc1;
    unsigned long crc2;
    
    // socklen_t clientAddressLength;
    struct sockaddr_storage fromAddr;
    fromAddrLength = sizeof(fromAddr);

    // FIRST PTYPE_DATA Packet Received
    int receivedBytes = recvfrom(sock, buffer, sizeof(buffer)-1, 0, (struct sockaddr*)&fromAddr, &fromAddrLength);


    if (receivedBytes == -1) { // this means transmission error? not the same as receiving a PTYPE DATA segment with field TR set to 1?
        // Previously cerr statement, now need to send PTYPE_NACK Packet? possibly another else if case for if sender packet has TR = 1
        /*
        myHeader->setBuffer(buffer2, BUFFER_SIZE);
        myHeader->setType(3); 
        myHeader->setTR(0);
        myHeader->setWindow(31);  //the field indicates the number of empty or available segments in the receiving window of the source host.
        myHeader->setSeqNum(1); 
        //myHeader->setTimeStamp(0);
        myHeader->setPayloadLength(0);
        crc1 = crc32(0L, NULL, 0);
        crc1 = crc32(crc1, reinterpret_cast<const Bytef*>(buffer), 8);
        myHeader->setCRC1(crc1);
        */
    }


    int payloadlen = receivedBytes - 16;
    // Send first PTYPE_ACK Packet
    myHeader->setBuffer(buffer2, ACK_SIZE);
    myHeader->setType(2); 
    myHeader->setTR(0);
    myHeader->setWindow(31);  //the field indicates the number of empty or available segments in the receiving window of the source host.
    myHeader->setSeqNum(1); 
    //myHeader->setTimeStamp(0);
    myHeader->setPayloadLength(0);
    crc1 = crc32(0L, NULL, 0);
    crc1 = crc32(crc1, reinterpret_cast<const Bytef*>(buffer), 8);
    myHeader->setCRC1(crc1);

    //write output to file
    buffer[receivedBytes] = 0;
    for (int i = 0; i < receivedBytes - 16; i++){
        stream << buffer[i + 12];
    }

    // Sendto used for bounceback
    sendto(sock, buffer2, sizeof(buffer2), 0, (struct sockaddr*)&fromAddr, fromAddrLength);

    
    // Now need 

    close(sock);
}


