//
// Created by Administrator on 2025/9/15.
//

#ifndef COMMON_H
#define COMMON_H
#include <bits/types.h>

// 协议号
typedef __uint8_t MSGId;

const MSGId AddItem = 1;
const MSGId RemoveItem = 2;
const MSGId UpdateItem = 3;
const MSGId GetItem = 4;

// 网络包结构
struct Packet {
    __uint16_t length; // 包长度
    MSGId msgNo; // 协议号
    unsigned char body[]; // 包体内容
};

// 道具结构
struct Item {
    __uint32_t itemId; // 道具id
    char itemName[20]; // 道具名
    __uint32_t itemNum; // 道具数量
};

// 协议包体
// 增加道具
struct AddItem {
    struct Item *item;
};

// 删除道具
struct DelItem {
    __uint32_t itemId; // 道具id
};

// 更改道具
struct UpdateItem {
    struct Item *item;
};

// 获取道具
struct GetItem {
    __uint32_t itemId; // 道具id
};

extern struct Packet* unmarshalDataToPacket(unsigned char* data, __uint16_t length, MSGId msgNo); // 序列化数据为包体
extern unsigned char* marshalPacketToData(struct Packet* packet); // 序列化包体为数据
extern __uint16_t getPacketLength(struct Packet* packet); // 获取包体长度
extern MSGId getPacketMsgNo(struct Packet* packet); // 获取协议号


#endif //COMMON_H
