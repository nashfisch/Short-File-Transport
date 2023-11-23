#ifndef __SIMPLEHEADER_H
#define __SIMPLEHEADER_H

#include <cstdint>
//Max payload size
#define DATA_SZ 1024
//Header size
#define HEADER_SZ 4


struct simplepacket {
  uint8_t header[HEADER_SZ];
  char data[DATA_SZ];  //payload
};


class SimpleHeader {
private:
  uint8_t * packet;  //Buffer Address
  int size;          //Size of buffer in bytes

  //Starting index of the payload
  const int PL=2;

  //Sarting index of the val
  const int VL=0;
  
public:
  //Default constructor
  SimpleHeader();

  //A buffer for the class. Setters and getters within the class will be used to modify its data.
  void setBuffer(void * buf, int sz);
  
  //Sets the length of payload
  void setPayloadLength(unsigned int val);

  //Returns the length of payload
  unsigned int getPayloadLength() const;

  //Returns the type. Can be 0-3, as it is a 2 bit field.
  unsigned int getType() const;

  //Sets the type. If it is not 0-3, it is set to 0.
  void setType(unsigned int val);

  unsigned int getTR() const;
  void setTR(unsigned int val);
  //Returns the window
  unsigned int getWindow() const;

  //Sets the window
  void setWindow(unsigned int val);

  //Returns the sequence number.
  unsigned int getSeqNum() const;

  //Sets the sequence number.
  void setSeqNum(unsigned int val);

  //Returns the length of the whole packet.
  unsigned int getLength() const;

  //Sets the length of the whole packet.
  void setLength(unsigned int val);

  //Returns the CRC1 packet. CRC1 is done on the first 8 bits.
  unsigned int getCRC1() const;

  //Sets the CRC1 of the packet. CRC1 applies to the whole packet.
  void setCRC1(unsigned int val);

  //Sets the time stamp.
  void setTimeStamp(unsigned int val);

  //Sets teh CRC2 of the packet. CRC2 applies to the just the payload.
  void setCRC2(unsigned int val, int payloadSize);

  // returns the size of the packet in bytes, including headers and data
  // to be used with recvfrom() or sendto()
  unsigned int getBufferSize() const {
    return size;
  }

  //Returns the packet itself. Used with sendto and rcvfrom.
  uint8_t* getBufferAddress() {
    return packet;
  }

  //Returns a pointer to the first byte of the payload.
  void * getPayloadAddress() {
    if (packet != nullptr && size > HEADER_SZ) {
      return packet+HEADER_SZ;
    } else {
      return nullptr;
    }
  }
};

#endif
