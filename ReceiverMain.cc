#include "Sender.h"
#include "Receiver.h"
#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <netdb.h>

int main(int argc, char* argv[]) {
    // Copied and pasted from sender
    std::ofstream Payload;
    std::string fileName;
    std::string IPv;
    std::string receiverPort;
    if (argc != 5 && argc > 1) {
        std::string fileCommand = argv[1];
        if (argc == 4 && fileCommand == "-f"){ //receiver -f filename port
            fileName = argv[2];
            IPv = 4;
            receiverPort = argv[3];
        } else if (argc == 3 && fileCommand == "-6") { // receiver -6 port
            fileName = "Payload.txt";
            IPv = fileCommand;
            receiverPort = argv[2];
        } else if (argc == 2) { // receiver port
            fileName = "Payload.txt";
            IPv = 4;
            receiverPort = fileCommand;
        } else {
            std::cerr << "Usage " << argv[0] << " [-6], receiver_port" << std::endl;
            return 1;
        }
    } 
    else if (argc == 5){
        std::string fileCommand = argv[1];
        std::string IPvCommand = argv[3];
        if (fileCommand == "-f" && IPvCommand == "-6"){
            fileName = argv[2];
            IPv = IPvCommand;
            receiverPort = argv[4];
        } else if (argc == 4 && fileCommand == "-f") {
                fileName = argv[2];
                IPv = 4;
                receiverPort = argv[3];
        } else {
            std::cerr << "Usage " << argv[0] << " [-f filename], port" << std::endl;
            return 1;
        }
    }


    Receiver receiver(receiverPort, IPv, 3);


    receiver.ReceiveFile(fileName, Payload);
    Payload.close();

    return 0;
}
