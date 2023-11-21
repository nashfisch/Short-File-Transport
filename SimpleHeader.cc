#include "SimpleHeader.h"

SimpleHeader::SimpleHeader() {
  // silly code, you can do better
  packet = nullptr;
  size = 0;
}

void SimpleHeader::setBuffer(void * buf, int sz) {
  packet = static_cast<uint8_t*> (buf);
  size = sz;
}

void SimpleHeader::setPayloadLength(unsigned int val) {
  if (packet != nullptr && size > HEADER_SZ) {
    packet[PL] = (val>>8); // shift the integer to right by 8 bits to get the msb
    packet[PL+1] = (val&255); // bitwise AND with 8 LSB bits set to 1.
  }
}

unsigned int SimpleHeader::getPayloadLength() const {
  if (packet != nullptr && size > HEADER_SZ) {
    return packet[PL+1] | (packet[PL]<<8);
  }
  else {
    return 0;
  }
}

unsigned int SimpleHeader::getType() const {
  if (packet != nullptr && size > HEADER_SZ) {
    return (packet[VL] >> 6);
  }
  else {
    return 0;
  }
}

 
void SimpleHeader::setType(unsigned int val) {
  if (packet != nullptr && size > HEADER_SZ) {
    // clear the val first
    packet[VL] &= 0x3f;

    // set the lowest 2 bits of val to the header field, but do not disturb the other bits
    packet[VL] |= (val << 6);
    // OBS: this implementation contains an error. It does not clear the header field if the value is > 3. Add a test for this case, see it fail, then fix the error
  }
}

unsigned int SimpleHeader::getTR() const {
  if (packet != nullptr && size > HEADER_SZ) {
    return (packet[0] >> 5) & 1;
  }
  else {
    return 0;
  }
}

void SimpleHeader::setTR(unsigned int val) {
  if (packet != nullptr && size > HEADER_SZ && val < 2) {
    packet[0] &= 0xDF; // clearing with 11011111

    packet[0] |= (val << 5); // setting
  }
}

unsigned int SimpleHeader::getWindow() const {
  if (packet != nullptr && size > HEADER_SZ) {
    return (packet[0] & 0x1f);
  }
  else {
    return 0; 
  }
}

void SimpleHeader::setWindow(unsigned int val) {
  if (packet != nullptr && size > HEADER_SZ && val < 32) {
    packet[0] &= 0xE0; // clearing with 11100000

    packet[0] |= (val); // setting
  }
}

unsigned int SimpleHeader::getSeqNum() const {
  if (packet != nullptr && size > HEADER_SZ) {
    return packet[1];
  }
  else {
    return 0;
  }
}

void SimpleHeader::setSeqNum(unsigned int val) {
  if (packet != nullptr && size > HEADER_SZ && val < 256) {
    packet[1] &= 0x00; // clearing with 0000000

    packet[1] |= (val); // setting
  }
}

unsigned int SimpleHeader::getLength() const {
  if (packet != nullptr && size > HEADER_SZ) {
    unsigned int temp; // need to access/store packet[2] and packet[3]
    temp = packet[2] << 8;
    temp |= packet[3];

    return temp;
  }
  else {
    return 0;
  }
}

void SimpleHeader::setLength(unsigned int val) {
  if (packet != nullptr && size > HEADER_SZ && val < 65536) {
    packet[2] &= 0x00;
    packet[3] &= 0x00;

    // setting
    packet[2] |= val >> 8;
    packet[3] |= (0xFF & val);
  }
}

unsigned int SimpleHeader::getCRC1() const {
  if (packet != nullptr && size > HEADER_SZ) {
    unsigned int temp;
    temp = packet[8] << 24;
    temp |= packet[9] << 16;
    temp |= packet[10] << 8;
    temp |= packet[11];

    return temp;
  }
  else {
    return 0;
  }
}

void SimpleHeader::setCRC1(unsigned int val) {
  if (packet != nullptr && size > HEADER_SZ) {
    packet[8] &= 0x00;
    packet[9] &= 0x00;
    packet[10] &= 0x00;
    packet[11] &= 0x00;

    packet[8] |= val >> 24;
    packet[9] |= (0xFF & val >> 16);
    packet[10] |= (0xFF & val >> 8);
    packet[11] |= (0xFF & val);
  }
}




