#ifndef RECEIVER_H
#define RECEIVER_H
#include "SlidingWindow.h"
#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <netdb.h>



class Receiver : public SlidingWindow {
public:
  /***
   * @brief Constructor
   * @param listenPort: The port number that the receiver listens to
   * @param IPvType: The specified IPv that should be used
   * @param max: max window size
  */
  Receiver(std::string& listenPort, std::string& IPvType, int max);
  //Receiver(int max);

  /***
   * @brief Initializes and binds the socket supplied by the constructor
  */
  void Initialize();

  /***
   * @brief Waits on the socket to receive a message from the sender, then 
   *        returns the same message back to the sender as an acknowledgement.
  */
  void ReceiveMessage(); 

  void ReceiveFile(std::string fileName);
    
private:
  // Port number being listened to
  std::string lPort;
  // Socket
  int sock;
  // Address of the sender
  sockaddr_in fromAddr;
  // Length of address of the sender
  socklen_t fromAddrLength;
  // String that stores the IPv that is being used. Should only ever be -4 or -6.
  std::string IPv = "-4";
  // Sequence number of the last in sequence frame received
  //int lastAck;
};

#endif
