#include "SimpleHeader.h"

SimpleHeader::SimpleHeader() {
  packet = nullptr;
  size = 0;
}

void SimpleHeader::setBuffer(void * buf, int sz) {
  packet = static_cast<uint8_t*> (buf);
  size = sz;
}

void SimpleHeader::setPayloadLength(unsigned int val) {
  if (packet != nullptr && size > HEADER_SZ) {
    packet[PL] = (val>>8);
    packet[PL+1] = (val&255);
  }
}

unsigned int SimpleHeader::getPayloadLength() const {
  if (packet != nullptr && size > HEADER_SZ) {
    return packet[PL+1] | (packet[PL]<<8);
  } else {
    return 0;
  }
}

unsigned int SimpleHeader::getType() const {
  if (packet != nullptr && size > HEADER_SZ) {
    return (packet[VL] >> 6);
  } else {
    return 0;
  }
}

 
void SimpleHeader::setType(unsigned int val) {
  if (packet != nullptr && size > HEADER_SZ) {
    packet[VL] &= 0x3f;
    packet[VL] |= (val << 6);
  }
}

unsigned int SimpleHeader::getTR() const {
  if (packet != nullptr && size > HEADER_SZ) {
    return (packet[0] >> 5) & 1;
  } else {
    return 0;
  }
}

void SimpleHeader::setTR(unsigned int val) {
  if (packet != nullptr && size > HEADER_SZ && val < 2) {
    packet[0] &= 0xDF;

    packet[0] |= (val << 5);
  }
}

unsigned int SimpleHeader::getWindow() const {
  if (packet != nullptr && size > HEADER_SZ) {
    return (packet[0] & 0x1f);
  } else {
    return 0; 
  }
}

void SimpleHeader::setWindow(unsigned int val) {
  if (packet != nullptr && size > HEADER_SZ && val < 32) {
    packet[0] &= 0xE0;

    packet[0] |= (val);
  }
}

unsigned int SimpleHeader::getSeqNum() const {
  if (packet != nullptr && size > HEADER_SZ) {
    return packet[1];
  } else {
    return 0;
  }
}

void SimpleHeader::setSeqNum(unsigned int val) {
  if (packet != nullptr && size > HEADER_SZ && val < 256) {
    packet[1] &= 0x00;
    
    packet[1] |= (val);
  }
}

unsigned int SimpleHeader::getLength() const {
  if (packet != nullptr && size > HEADER_SZ) {
    unsigned int temp;
    temp = packet[2] << 8;
    temp |= packet[3];

    return temp;
  } else {
    return 0;
  }
}

void SimpleHeader::setLength(unsigned int val) {
  if (packet != nullptr && size > HEADER_SZ && val < 65536) {
    packet[2] &= 0x00;
    packet[3] &= 0x00;

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
  } else {
    return 0;
  }
}

void SimpleHeader::setTimeStamp(unsigned int val) {
  if (packet != nullptr && size > HEADER_SZ) {
    packet[4] &= 0x00;
    packet[5] &= 0x00;
    packet[6] &= 0x00;
    packet[7] &= 0x00;

    packet[4] |= val >> 24;
    packet[5] |= (0xFF & val >> 16);
    packet[6] |= (0xFF & val >> 8);
    packet[7] |= (0xFF & val);
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


void SimpleHeader::setCRC2(unsigned int val, int payloadSize) {
  if (packet != nullptr && size > HEADER_SZ) {
    packet[payloadSize + 12] &= 0x00;
    packet[payloadSize + 13] &= 0x00;
    packet[payloadSize + 14] &= 0x00;
    packet[payloadSize + 15] &= 0x00;

    packet[payloadSize + 12] |= val >> 24;
    packet[payloadSize + 13] |= (0xFF & val >> 16);
    packet[payloadSize + 14] |= (0xFF & val >> 8);
    packet[payloadSize + 15] |= (0xFF & val);
  }
}




