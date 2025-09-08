//
// Created by zjyan on 2025/8/13.
//


#ifndef SERVER_H
#define SERVER_H
#include <sys/types.h>

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
    char body[]; // 包体内容
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

// socket处理
extern void* processSocket(void* socket);

// 网络消息处理
extern struct Packet* parsePacket(char data[]); // 解析字节序为包体
extern int sendPacket(int socket, struct Packet *packet); // 发送网络消息包
extern int recvPacket(int socket, char data[]); // 接收网络消息
extern int handlePacket(int socket, struct Packet *packet); // 处理消息包
extern int parseBody(int socket, char data[]);


// 具体协议方法
extern int addItem(struct Item *item);
extern int removeItem(__uint32_t itemId);
extern int updateItem(struct Item *item);
extern int getItem(__uint32_t itemId);

#endif //SERVER_H
