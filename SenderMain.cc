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

    std::cout << "Gets to creating sender" << std::endl;    
    Sender sender(senderDes, senderPort, 3);
    std::cout << "Creates sender" << std::endl;    
    /*
    std::cout << "Enter a message: ";
    std::getline(std::cin, message);
    sender.SendMessage(message);
    */
    std::cout << "Gets to creating header" << std::endl; 

    SimpleHeader* myHeader = new SimpleHeader();
    myHeader->setType(1);
    myHeader->setTR(0);
    myHeader->setWindow(1);
    myHeader->setSeqNum(0);
    myHeader->setLength(512);

    std::cout << "Fills in header" << std::endl; 
   
    caster = myHeader->getBufferAddress();
    std::cout << "u_int8* Rep: ";
    
    for (int i = 0; i < 2; i++){
        std::cout << caster[i];
    }
    
    
    header = (char *) caster;
    std::cout << "Casts header" << std::endl; 
    std::cout << "char* Rep: " << header << std::endl;

    unsigned long crc;
    std::cout << "CRC Line 1" << std::endl;
    crc = crc32(0L, NULL, 0);
    std::cout << "CRC Line 2" << std::endl;
    crc = crc32(crc, reinterpret_cast<const Bytef*>(header), strlen(header));
    std::cout << "CRC Line 3" << std::endl;
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

    delete myHeader;
    delete header;
    delete caster;


    return 0;
}
