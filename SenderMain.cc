#include "Sender.h"
#include "Receiver.h"
#include "SimpleHeader.h"
#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <netdb.h>
#include <zlib.h>

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage " << argv[0] << " sender_host, sender_port, sender_filename" << std::endl;
        return 1;
    }

    std::string senderDes = argv[1];
    std::string senderPort = argv[2];
    std::string senderFile = argv[3];
    std::string message = "";
    char* header;
    uint8_t* caster;


    
    Sender sender(senderDes, senderPort, 3);
    /*
    std::cout << "Enter a message: ";
    std::getline(std::cin, message);
    sender.SendMessage(message);
    */
    SimpleHeader* myHeader = new SimpleHeader();
    myHeader->setType(1);
    myHeader->setTR(0);
    myHeader->setWindow(1);
    myHeader->setSeqNum(0);
    myHeader->setLength(512);
    caster = myHeader->getBufferAddress();

    header = (char *) caster;

    unsigned long crc;
    crc = crc32(0L, NULL, 0);
    crc = crc32(crc, reinterpret_cast<const Bytef*>(header), strlen(header));
    std::cout << "The crc32 value for: " << &header << " is " << std::hex << crc << std::endl;
    myHeader->setCRC1(crc);

    std::ifstream ifi;
    ifi.open("Receiver.cc", std::ios::binary);
    if(ifi.fail()) {
        std::cout << "Failed to open file." << std::endl;
        exit(1);
    }

    char payload[512];
    ifi.read(payload, 512);

    if (ifi) {
        std::cout << "All characters read." << std::endl;
    } else {
        std::cout << "File too large. Cutoff at 512 bytes." << std::endl;
    }
    
    unsigned int plug = myHeader->getLength();
    header[plug] = *payload;

   
    sender.SendMessage(header);




    return 0;
}
