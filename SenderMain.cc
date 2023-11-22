#include "Sender.h"
#include "Receiver.h"
#include "SimpleHeader.h"
#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <iomanip>
#include <netdb.h>
#include <zlib.h>

#define BUFFER_SIZE 528
#define PAYLOAD_SIZE 512

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage " << argv[0] << " sender_host, sender_port, sender_filename" << std::endl;
        return 1;
    }

    std::string senderDes = argv[1];
    std::string senderPort = argv[2];
    std::string senderFile = argv[3];
    char* header = nullptr;
    uint8_t* buffer = new uint8_t[BUFFER_SIZE];
    uint8_t* goodStuff = nullptr;


    Sender sender(senderDes, senderPort, 3);

    /*
    std::cout << "Enter a message: ";
    std::getline(std::cin, message);
    sender.SendMessage(message);
    */


    SimpleHeader* myHeader = new SimpleHeader();
    myHeader->setBuffer(buffer, BUFFER_SIZE);

    myHeader->setType(1); 
    myHeader->setTR(0);
    myHeader->setWindow(0);
    myHeader->setSeqNum(0); // header[1]
    myHeader->setPayloadLength(0x0200); // header[2] and header[3]
    myHeader->setTimeStamp(0);
    
  


    unsigned long crc1;
    unsigned long crc2;

    crc1 = crc32(0L, NULL, 0);
    crc1 = crc32(crc1, reinterpret_cast<const Bytef*>(buffer), 8);

    std::cout << "The CRC1 crc32 value for: " << buffer << " is " << std::hex << crc1 << std::endl;
    myHeader->setCRC1(crc1); // header[4] - header[8]

    for (int i = 0; i < 12; i++){
        std::cout << "BUFFER 1: " << std::hex <<std::setw(2) << std::setfill('O') << static_cast<int>(buffer[i]) << std::endl;
    }
   



    std::ifstream ifi;
    ifi.open("Receiver.cc", std::ios::binary);
    if(ifi.fail()) {
        std::cout << "Failed to open file." << std::endl;
        exit(1);
    }

    char payload[PAYLOAD_SIZE];
    ifi.read(payload, sizeof(payload));
    std::streamsize bytesRead = ifi.gcount();
  

    for (int i = 12; i < bytesRead; i++){
        buffer[i] = payload[i - 12];
    }
    goodStuff = &buffer[12];

    if (ifi) {
        std::cout << "All characters read." << std::endl;
    } else {
        std::cout << "File too large. Cutoff at 512 bytes." << std::endl;
    }


    crc2 = crc32(0L, NULL, 0);
    crc2 = crc32(crc2, reinterpret_cast<const Bytef*>(goodStuff), PAYLOAD_SIZE);
    std::cout << "The CRC2 crc32 value for: " << goodStuff << " is " << std::hex << crc2 << std::endl;
    myHeader->setCRC2(crc2);
    header = reinterpret_cast<char*>(buffer);
    
    for (int i = 0; i < 528; i++){
        std::cout << "HEADER: " << std::hex <<std::setw(2) << std::setfill('O') << static_cast<int>(header[i]) << std::endl;
    }
    
    int packetLength = 16 + bytesRead;
    sender.SendMessage(header, packetLength);

    delete myHeader;
    delete[] header;
    delete[] buffer;
    goodStuff = nullptr;
    myHeader = nullptr;
    header = nullptr;
    buffer = nullptr;


    return 0;
}
