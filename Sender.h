#ifndef SENDER_H
#define SENDER_H
#include "SlidingWindow.h"
#include "SimpleHeader.h"
#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <netdb.h>

class Sender : public SlidingWindow {
public:
  /***
   * @brief Constructor for Sender
   * @param destinationHost: The host name of where the message is to be sent
   * @param destinationPort: The port # of the destination (receiver)
   * @param max: Ignored value included in parent class constructor.
  */
  Sender(std::string& destinationHost, std::string& destinationPort, int max);

  /***
   * @brief Initializes and binds the socket supplied by the constructor
  */
  void Initialize();

  /***
   * @brief Sends a message to the receiver.
   * @param message: The message that is to be sent
  */
  void SendMessage(char* message, int packetLength, SimpleHeader* header);

  //void SendFile(std::string& fileName);

  //bool req(int SN);
  //int retransmit();
  


private:
  // Host name of receiver
  std::string destHost;
  // Port # of receiver
  std::string destPort;


  // Socket to connect to receiver
  int sock;
  // Gives us address and information
  struct addrinfo* destAddr;
    
};

#endif