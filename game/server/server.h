//
// Created by zjyan on 2025/8/13.
//


#ifndef SERVER_H
#define SERVER_H
#include "../common/common.h"

// 服务器进程相关
extern int loadItemData();

extern int exitServer();

// socket处理
extern void *processSocket(void *socket);

// 网络消息处理
extern int sendPacket(int socket, struct Packet *packet); // 发送网络消息包
extern int recvPacket(int socket, char data[]); // 接收网络消息
extern int handlePacket(int socket, const struct Packet *packet); // 处理消息包
extern int parseBody(int socket, char data[]);


// 具体协议方法
extern int addItem(unsigned char *body, PACKET_LEN length, int socket);

extern int removeItem(unsigned char *body, PACKET_LEN length, int socket);

extern int updateItem(unsigned char *body, PACKET_LEN length, int socket);

extern int getItem(unsigned char *body, PACKET_LEN length, int socket);

#endif //SERVER_H
