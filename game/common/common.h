//
// Created by Administrator on 2025/9/15.
//

#ifndef COMMON_H
#define COMMON_H
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <bits/types.h>

// 协议号
typedef __uint8_t MSGId;

enum MessageIds {
    MIN_MSG_NO = 0, // 最小消息ID
    AddItem = 1, // 添加道具
    RemoveItem = 2, // 删除道具
    UpdateItem = 3, // 更新道具
    GetItem = 4, // 获取道具
    MAX_MSG_NO = 255 // 最大消息ID
};

typedef __uint16_t PACKET_LEN;
extern const __uint16_t PACKET_HEADER_SIZE;

// 网络包结构
struct Packet {
    PACKET_LEN length; // 包长度
    MSGId msgNo; // 协议号
    unsigned char *body; // 包体内容
};

extern const char fileName[];

FILE *itemFile = NULL;

extern struct Item *getItemFromFileByPos(int pos); // 获取道具从某个位置
extern int cleanFile(); // 清除文件内容
extern int addItemToFile(const struct Item *item); // 添加道具到文件中
extern int getItemFileSize(); // 获取道具文件大小

#define TABLE_SIZE 10000

typedef struct Node {
    struct Item *item;
    struct Node *next;
} Node;

typedef struct HashTable {
    __uint32_t size;
    __uint32_t capacity;
    Node **table;
} HashTable;

HashTable *ItemHashTable;

extern int initHashTable(__uint32_t capacity); // 初始化道具哈希表
extern __uint32_t hash(__uint32_t key); // 哈希函数
extern struct Item *getItem(__uint32_t id); // 获取道具
extern bool putItem(struct Item *item); // 放入道具
extern bool removeItem(__uint32_t id); // 删除道具
extern void freeItemHashTable(); // 释放道具哈希表内存
extern struct Item *getAllItemsFromHashTable(); // 获取所有道具

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

extern struct Packet *unmarshalDataToPacket(const unsigned char *data, __uint16_t length, MSGId msgNo); // 序列化数据为包体
extern unsigned char *marshalPacketToData(struct Packet *packet); // 序列化包体为数据

extern int isNumber(const char *str); // 判断字符串是否为数字
extern long convertToLong(const char *str); // 字符串转换为long类型

extern bool getValidNumber(const char *prompt, __uint32_t *result, __uint32_t minValue, __uint32_t maxValue);

extern bool getValidString(const char *prompt, char *result, int maxLength);

#endif //COMMON_H
