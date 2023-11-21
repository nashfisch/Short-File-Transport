#ifndef __SIMPLEHEADER_H
#define __SIMPLEHEADER_H

#include <cstdint>
// maximum size for the payload 
#define DATA_SZ 1024
// size of header
#define HEADER_SZ 4


struct simplepacket {
  uint8_t header[HEADER_SZ];
  char data[DATA_SZ];  // payload
};

// class to be tested. Implements the demo header structure below.
// exports routines to get and set the header parameters val (integer between 0 and 3) and payload_length (16 bit unsigned integer) for a buffer containing the packet
//  0 1  2 7   8 15  16   32
// +---+-----+-----+--------+
// |Val| unu | unu |payload |
// |   | sed | sed |length  |
// +---+-----+-----+--------+
// NOTE: this class does not allocate memory for the packet buffer. Memory for the buffer must be already allocated by the calling code.

class SimpleHeader {
private:
  uint8_t * packet;  // address of the buffer
  int size;          // size of buffer in bytes

  // start index of the payload length field (private)
  const int PL=2;

  // start index of val header field
  const int VL=0;
  
public:
  // default constructor, packet buffer address set to NULL
  SimpleHeader();

  // assigns an externally allocated buffer to the class; the set/get member functions will manage the headers fields for this buffer
  // buf = the address of the buffer
  // sz = the size in bytes of the buffer
  void setBuffer(void * buf, int sz);
  
  // sets the value of the payload length
  // val = length; if val > DATA_SZ, the value set is DATA_SZ
  // if buffer address is NULL, does nothing.
  void setPayloadLength(unsigned int val);

  // returns the length of the payload
  // if buffer address is NULL, returns 0
  unsigned int getPayloadLength() const;

  // returns the val field of the header. Must be between 0..3 since
  // it is a 2 bit value
  // if buffer address is NULL, returns 0
  unsigned int getType() const;

  // sets the value field of the header.
  // If the val is not between 0..3, the value set is 0
  // if buffer address is NULL, does nothing
  void setType(unsigned int val);

  unsigned int getTR() const;
  void setTR(unsigned int val);
  unsigned int getWindow() const;
  void setWindow(unsigned int val);
  unsigned int getSeqNum() const;
  void setSeqNum(unsigned int val);
  unsigned int getLength() const;
  void setLength(unsigned int val);
  unsigned int getCRC1() const;
  void setCRC1(unsigned int val);


  // returns the size of the packet in bytes, including headers and data
  // to be used with recvfrom() or sendto()
  unsigned int getBufferSize() const {
    return size;
  }

  // returns pointer to the structure holding the thePacket, including the headers
  // To be used with recvfrom or sendto
  uint8_t* getBufferAddress() {
    return packet;
  }

  // returns a pointer to the first byte of the payload from the packet buffer;
  // returns NULL if the buffer is NULL
  void * getPayloadAddress() {
    if (packet != nullptr && size > HEADER_SZ) {
      return packet+HEADER_SZ;
    } else {
      return nullptr;
    }
  }
};

#endif
