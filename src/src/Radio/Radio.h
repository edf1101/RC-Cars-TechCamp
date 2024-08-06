#ifndef RADIO_H
#define RADIO_H

#include <FastCRC.h>
#include <functional>

struct datapacketstruct {
    uint16_t crc;
    uint16_t unitNum;
    int8_t throttle;
    uint8_t angle;
    uint8_t command;
} __attribute__ ((packed));

void setupRadio(int channel, std::function<void()> _callback);

void sendPacket();

extern datapacketstruct incomingRadioPacket, outgoingRadioPacket;

#endif // RADIO_H
