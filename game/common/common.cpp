//
// Created by Administrator on 2025/9/15.
//
#include "common.h"

#include <stdlib.h>
#include <string.h>

struct Packet* unmarshalDataToPacket(unsigned char* data, __uint16_t length, MSGId msgNo) {
    struct Packet* packet = {0};
    packet->length = length;
    packet->msgNo = msgNo;
    memcpy(packet->body, data, length);
    return packet;
}

unsigned char* marshalPacketToData(struct Packet* packet) {
    unsigned char* data = (unsigned char*)malloc(packet->length);
}