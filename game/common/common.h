//
// Created by Administrator on 2025/9/15.
//

#ifndef COMMON_H
#define COMMON_H
#include <stdbool.h>
#include <stddef.h>
#include <bits/types.h>

// 协议号
typedef __uint8_t MSGId;

enum MessageIds {
    MIN_MSG_NO = 0,    // 最小消息ID
    AddItem = 1,       // 添加道具
    RemoveItem = 2,    // 删除道具
    UpdateItem = 3,    // 更新道具
    GetItem = 4,       // 获取道具
    MAX_MSG_NO = 255   // 最大消息ID
};

typedef __uint16_t PACKET_LEN;
extern const __uint16_t PACKET_HEADER_SIZE;

// 网络包结构
struct Packet {
    PACKET_LEN length; // 包长度
    MSGId msgNo; // 协议号
    unsigned char* body; // 包体内容
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
    struct Item item;
};

// 删除道具
struct DelItem {
    __uint32_t itemId; // 道具id
};

// 更改道具
struct UpdateItem {
    struct Item item;
};

// 获取道具
struct GetItem {
    __uint32_t itemId; // 道具id
};

extern struct Packet* unmarshalDataToPacket(const unsigned char* data, __uint16_t length, MSGId msgNo); // 序列化数据为包体
extern unsigned char* marshalPacketToData(struct Packet* packet); // 序列化包体为数据

extern int isNumber(const char* str); // 判断字符串是否为数字
extern long convertToLong(const char* str); // 字符串转换为long类型

extern bool getValidNumber(const char* prompt,__uint32_t* result,__uint32_t minValue,__uint32_t maxValue);
extern bool getValidString(const char* prompt,char* result,int maxLength);

#endif //COMMON_H
