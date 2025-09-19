//
// Created by Administrator on 2025/9/15.
//
#include "common.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// 变量定义
const __uint16_t PACKET_HEADER_SIZE = sizeof(PACKET_LEN) + sizeof(MSGId);
const char fileName[] = "item.dat";

struct Packet *unmarshalDataToPacket(const unsigned char *data, __uint16_t length, MSGId msgNo) {
    struct Packet *packet = malloc(sizeof(struct Packet));
    if (packet == NULL) {
        return NULL;
    }

    packet->length = length;
    packet->msgNo = msgNo;
    packet->body = malloc(length);
    if (packet->body == NULL) {
        free(packet);
        return NULL;
    }
    memcpy(packet->body, data, length);
    return packet;
}

unsigned char *marshalPacketToData(struct Packet *packet) {
    if (packet == NULL) {
        printf("marshalPacketToData packet is NULL");
        return NULL;
    }
    const u_int16_t dataLength = PACKET_HEADER_SIZE + packet->length;
    unsigned char *data = malloc(dataLength);
    if (data == NULL) {
        printf("marshalPacketToData malloc failed");
        return NULL;
    }

    size_t offset = 0;
    memcpy(data + offset, &packet->length, sizeof(PACKET_LEN));
    offset += sizeof(PACKET_LEN);
    memcpy(data + offset, &packet->msgNo, sizeof(MSGId));
    offset += sizeof(MSGId);
    memcpy(data + offset, packet->body, packet->length);
    return data;
}

int isNumber(const char *str) {
    if (str == NULL) {
        printf("isNumber str is NULL");
        return 0;
    }
    for (int i = 0; i < strlen(str) - 1; i++) {
        if (str[i] < '0' || str[i] > '9') {
            return 0;
        }
    }
    return 1;
}

long convertToLong(const char *str) {
    if (str == NULL) {
        printf("convertToLong str is NULL");
        return -1;
    }

    char *endptr;
    long result = strtol(str, &endptr, 10);
    if (endptr == str) {
        printf("convertToLong not number");
        return -1;
    }
    if (*endptr != '\0' && *endptr != '\n') {
        printf("convertToLong not number");
        return -1;
    }
    return result;
}

bool getValidNumber(const char *prompt, __uint32_t *result, __uint32_t minValue, __uint32_t maxValue) {
    printf("%s", prompt);

    while (true) {
        char buff[12];
        bzero(buff, sizeof(buff));
        if (fgets(buff, sizeof(buff),stdin) == NULL) {
            printf("输入错误，请重新输入\n");
            continue;
        }

        if (!isNumber(buff)) {
            printf("必须输入数字，请重新输入：");
            continue;
        }

        long num = convertToLong(buff);
        if (num < minValue || num > maxValue) {
            printf("输入数字不在范围内，请重新输入：");
            continue;
        }

        *result = (__uint32_t) num;
        return true;
    }
}

bool getValidString(const char *prompt, char *result, int maxLength) {
    printf("%s", prompt);

    while (true) {
        if (fgets(result, maxLength + 1,stdin) == NULL) {
            printf("输入错误，请重新输入\n");
            continue;
        }

        // 去除换行符
        char *newLine = strchr(result, '\n');
        if (newLine != NULL) {
            *newLine = '\0';
        }

        // 检查是否为空
        if (strlen(result) == 0) {
            printf("输入不能为空，请重新输入\n");
            continue;
        }
        return true;
    }
}

int initHashTable(__uint32_t capacity) {
    ItemHashTable = malloc(sizeof(HashTable));
    if (ItemHashTable == NULL) {
        printf("initHashTable malloc failed");
        return -1;
    }

    ItemHashTable->size = 0;
    ItemHashTable->capacity = capacity;
    ItemHashTable->table = malloc(sizeof(Node *) * capacity);
    if (ItemHashTable->table == NULL) {
        printf("initHashTable malloc table failed");
        return -1;
    }
    for (int i = 0; i < capacity; i++) {
        ItemHashTable->table[i] = NULL;
    }
    return 0;
}

__uint32_t hash(__uint32_t key) {
    // 使用黄金分割比例的乘法哈希
    key = ((key >> 16) ^ key) * 0x45d9f3b;
    key = ((key >> 16) ^ key) * 0x45d9f3b;
    key = (key >> 16) ^ key;
    return key % ItemHashTable->capacity;
}

struct Item *getItem(__uint32_t id) {
    __uint32_t key = hash(id);
    Node *node = ItemHashTable->table[key];
    if (node == NULL) {
        printf("getItem key: %d, node is NULL", key);
        return NULL;
    }

    while (node != NULL) {
        if (node->item->itemId == id) {
            return node->item;
        }
        node = node->next;
    }
    return NULL;
}

bool putItem(struct Item *item) {
    if (item == NULL) {
        printf("putItem item is NULL");
        return false;
    }

    __uint32_t key = hash(item->itemId);
    Node *node = ItemHashTable->table[key];
    if (node == NULL) {
        node = malloc(sizeof(Node));
        if (node == NULL) {
            printf("putItem malloc node failed");
            return false;
        }

        node->item = item;
        node->next = NULL;
        ItemHashTable->table[key] = node;
        ItemHashTable->size++;
        return true;
    }

    while (node->next != NULL) {
        if (node->item->itemId == item->itemId) {
            free(node->item);
            node->item = item;
            return true;
        }
        node = node->next;
    }
    Node *newNode = malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("putItem malloc newNode failed");
        return false;
    }

    newNode->item = item;
    newNode->next = NULL;
    node->next = newNode;
    ItemHashTable->size++;
    return true;
}

bool removeItem(__uint32_t id) {
    __uint32_t key = hash(id);
    Node *node = ItemHashTable->table[key];
    if (node == NULL) {
        printf("removeItem key: %d, node is NULL", key);
        return false;
    }

    if (node->item->itemId == id) {
        ItemHashTable->table[key] = node->next;
        free(node->item);
        free(node);
        ItemHashTable->size--;
        return true;
    }

    while (node->next != NULL) {
        Node *pre = node;
        Node *cur = node->next;
        if (cur->item->itemId == id) {
            pre->next = cur->next;
            free(cur->item);
            free(cur);
            ItemHashTable->size--;
            return true;
        }
        node = cur;
    }

    printf("removeItem: Item with id %u not found\n", id);
    return false;
}

void freeItemHashTable() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        Node *node = ItemHashTable->table[i];
        // 遍历当前桶的所有节点，释放内存
        while (node != NULL) {
            Node *next = node->next;
            // 释放节点关联结构体
            if (node->item != NULL) {
                free(node->item);
            }

            free(node);
            node = next;
        }
        ItemHashTable->table[i] = NULL;
    }

    ItemHashTable->size = 0;
    ItemHashTable->capacity = 0;
    free(ItemHashTable->table);
    free(ItemHashTable);
}

struct Item *getAllItemsFromHashTable() {
    struct Item *list = malloc(sizeof(struct Item) * ItemHashTable->size);
    if (list == NULL) {
        printf("getAllItemsFromHashTable malloc failed");
        return NULL;
    }
    int index = 0;
    for (int i = 0; i < TABLE_SIZE; i++) {
        if (ItemHashTable->table[i] == NULL) {
            continue;
        }
        Node *node = ItemHashTable->table[i];
        while (node != NULL) {
            list[index] = *node->item;
            node = node->next;
            index++;
        }
    }
    return list;
}


struct Item *getItemFromFileByPos(int pos) {
    struct Item *item = malloc(sizeof(struct Item));
    if (item == NULL) {
        printf("getItemFromFileByPos malloc item is NULL");
        return NULL;
    }

    int ret = fseek(itemFile, pos, SEEK_SET);
    if (ret != 0) {
        printf("getItemFromFileByPos fseek failed");
        return NULL;
    }
    size_t size = fread(item, sizeof(struct Item), 1, itemFile);
    if (size != 1) {
        printf("getItemFromFileByPos fread failed");
        return NULL;
    }
    return item;
}

int cleanFile() {
    int ret = fseek(itemFile, 0, SEEK_SET);
    if (ret != 0) {
        printf("cleanFile fseek failed");
        return -1;
    }
    ret = ftruncate(fileno(itemFile), 0);
    if (ret != 0) {
        printf("cleanFile ftruncate failed");
        return -1;
    }
    ret = fclose(itemFile);
    if (ret != 0) {
        printf("cleanFile fclose failed");
        return -1;
    }

    itemFile = NULL;
    return 0;
}

int addItemToFile(const struct Item *item) {
    const int ret = fseek(itemFile, 0, SEEK_SET);
    if (ret != 0) {
        printf("addItemToFile fseek failed");
        return -1;
    }
    const size_t size = fwrite(item, sizeof(struct Item), 1, itemFile);
    if (size != 1) {
        printf("addItemToFile fwrite failed");
        return -1;
    }
    return 0;
}

int getItemFileSize() {
    const int ret = fseek(itemFile, 0, SEEK_END);
    if (ret != 0) {
        printf("getItemFileSize fseek failed");
        return -1;
    }
    const long size = ftell(itemFile);
    if (size == -1) {
        printf("getItemFileSize ftell failed");
        return -1;
    }
    return (int) size;
}
