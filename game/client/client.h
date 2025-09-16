//
// Created by zjyan on 2025/8/18.
//

#ifndef CLIENT_H
#define CLIENT_H
#include "../common/common.h"

// 处理用户输入线程
extern void* processInput(void* socket);

extern void handleInput(MSGId msgNo,int socket);
extern struct Packet* handleAddItem();
extern struct Packet* handleRemoveItem();
extern struct Packet* handleUpdateItem();
extern struct Packet* handleGetItem();

#endif //CLIENT_H
