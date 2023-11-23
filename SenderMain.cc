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
#define PAYLOAD_MAX 512

int main(int argc, char* argv[]) {
    char payload[PAYLOAD_MAX];
    std::string senderFile;
    std::string senderDes;
    std::string senderPort;
    std::string contents;
    int packetLength = 0;
    int payloadSize = 0;

    char* header = nullptr;
    uint8_t* buffer = new uint8_t[BUFFER_SIZE];
    uint8_t* goodStuff = nullptr;

    SimpleHeader* myHeader = new SimpleHeader();
    myHeader->setBuffer(buffer, BUFFER_SIZE);

    myHeader->setType(1); 
    myHeader->setTR(0);
    myHeader->setWindow(0);
    myHeader->setSeqNum(0); // header[1]
    myHeader->setTimeStamp(0);
    bool args3;

    std::string fileCommand = argv[1];
    
    if (argc != 5) {
        if (argc == 3 && argv[1] != "-f") {
            std::cout << "Enter a message: ";
            std::getline(std::cin, contents);

            payloadSize = contents.size();

            

            
            for (int i = 12; i < payloadSize + 12; i++){
                buffer[i] = contents.c_str()[i - 12];
            }
            senderDes = argv[1];
            senderPort = argv[2];
            args3 = 1;

        } else {
            std::cerr << "Usage " << argv[0] << "sender_host, sender_port" << std::endl;
            return 1;
        }
        
    } else if (argc == 5 && fileCommand == "-f") {
        senderFile = argv[2];
        senderDes = argv[3];
        senderPort = argv[4];

        std::ifstream ifi;

        ifi.open(senderFile, std::ios::binary);
        if(ifi.fail()) {
            std::cout << "Failed to open file." << std::endl;
            exit(1);
        }

        ifi.read(payload, PAYLOAD_MAX);
        payloadSize = ifi.gcount();
        for (int i = 12; i < payloadSize + 12; i++){
            buffer[i] = payload[i - 12];
        }

        if (ifi.eof()) {
            std::cout << "All characters read." << std::endl;
        } else {
            std::cout << "File too large. Cutoff at 512 bytes." << std::endl;
        }
    } else {
        std::cerr << "Usage " << argv[0] << "[-f sender_filename] sender_host, sender_port" << std::endl;
        return 1;
    }
    myHeader->setPayloadLength(payloadSize); // header[2] and header[3]
    Sender sender(senderDes, senderPort, 3);

    unsigned long crc1;
    unsigned long crc2;

    crc1 = crc32(0L, NULL, 0);
    crc1 = crc32(crc1, reinterpret_cast<const Bytef*>(buffer), 8);
 
    myHeader->setCRC1(crc1); // header[4] - header[8]

    goodStuff = &buffer[12];



    crc2 = crc32(0L, NULL, 0);
    crc2 = crc32(crc2, reinterpret_cast<const Bytef*>(goodStuff), payloadSize);

    myHeader->setCRC2(crc2, payloadSize);

    header = reinterpret_cast<char*>(buffer);

    packetLength = 16 + payloadSize;
    sender.SendMessage(header, packetLength, myHeader);

    delete myHeader;
    delete[] buffer;
    buffer = nullptr;
    goodStuff = nullptr;
    myHeader = nullptr;
    header = nullptr;

    return 0;
}