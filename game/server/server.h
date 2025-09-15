//
// Created by zjyan on 2025/8/13.
//


#ifndef SERVER_H
#define SERVER_H
#include "../common/common.h"

// socket处理
extern void* processSocket(void* socket);

// 网络消息处理
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
