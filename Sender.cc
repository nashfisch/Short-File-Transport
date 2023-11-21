#include <iostream>
#include <vector>
#include "Sender.h"
#include "SlidingWindow.h"
#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
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

void Sender::SendMessage(char* message) {
    
    if (sendto(sock, message, strlen(message), 0, destAddr->ai_addr, destAddr->ai_addrlen) == -1) {
        std::cerr << "Error sending the message" << std::endl;
        exit(1);
    }
    char buffer[1024];
    socklen_t clientAddressLength;
    struct sockaddr_storage fromaddr;
    clientAddressLength = sizeof(fromaddr);
    
    int receivedBytes = recvfrom(sock, buffer, sizeof(buffer)-1, 0, (struct sockaddr*)&fromaddr, &clientAddressLength);
    if (receivedBytes == -1) {
        std::cerr << "Error receiving data" << std::endl;
    }
    buffer[receivedBytes] = 0;
    std::cout << "Received: " << buffer << std::endl;
}


/*
void Sender::SendFile(std::string& fileName) {
    

    REMOVING, relocating to SenderMain. New functionality of SendFile will be just sending the finalized packet.
    
    std::ifstream ifi;
    ifi.open(fileName, std::ios::binary);
    if(ifi.fail()) {
        std::cout << "Failed to open file." << std::endl;
        exit(1);
    }

    char buffer[512];
    ifi.read(buffer, 512);

    if (ifi) {
        std::cout << "All characters read." << std::endl;
    } else {
        std::cout << "File too large. Cutoff at 512 bytes." << std::endl;
    }

    std::string temp;
    temp = buffer;

     if (!ifi.eof()) {
        std::cout << "Error reading from file." << std::endl;
     } else {
        std::cout << "End of file reached." << std::endl;
     }
     

     for (int i = 0; i < 512; i++) {
        std::cout << buffer[i] << std::endl;
     }
     std::cout<< ifi.gcount() << std::endl;
     ifi.close();
     return temp;
}
*/
