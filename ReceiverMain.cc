#include "Sender.h"
#include "Receiver.h"
#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <netdb.h>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage " << argv[0] << " receiver_port, IPv# (-4 or -6)" << std::endl;
        return 1;
    }
    std::string receiverPort = argv[1];
    std::string IPv = argv[2];


    Receiver receiver(receiverPort, IPv, 3);


    receiver.ReceiveMessage();


    return 0;
}
