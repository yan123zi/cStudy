//
// Created by Administrator on 2025/9/15.
//
#include "common.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 变量定义
const __uint16_t PACKET_HEADER_SIZE = sizeof(PACKET_LEN) + sizeof(MSGId);

struct Packet* unmarshalDataToPacket(const unsigned char* data, __uint16_t length, MSGId msgNo) {
    struct Packet* packet = malloc(sizeof(struct Packet));
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

unsigned char* marshalPacketToData(struct Packet* packet) {
    if (packet == NULL) {
        printf("marshalPacketToData packet is NULL");
        return NULL;
    }
    const u_int16_t dataLength = PACKET_HEADER_SIZE+packet->length;
    unsigned char* data = malloc(dataLength);
    if (data == NULL) {
        printf("marshalPacketToData malloc failed");
        return NULL;
    }

    size_t offset = 0;
    memcpy(data+offset, &packet->length,sizeof(PACKET_LEN));
    offset += sizeof(PACKET_LEN);
    memcpy(data+offset, &packet->msgNo,sizeof(MSGId));
    offset += sizeof(MSGId);
    memcpy(data+offset, packet->body,packet->length);
    return data;
}

int isNumber(const char* str) {
    if (str == NULL) {
        printf("isNumber str is NULL");
        return 0;
    }
    for (int i = 0; i < strlen(str)-1; i++) {
        if (str[i] < '0' || str[i] > '9') {
            return 0;
        }
    }
    return 1;
}

long convertToLong(const char* str) {
    if (str == NULL) {
        printf("convertToLong str is NULL");
        return -1;
    }

    char* endptr;
    long result = strtol(str, &endptr, 10);
    if (endptr==str) {
        printf("convertToLong not number");
        return -1;
    }
    if (*endptr != '\0'&&*endptr != '\n') {
        printf("convertToLong not number");
        return -1;
    }
    return result;
}

bool getValidNumber(const char* prompt,__uint32_t* result,__uint32_t minValue,__uint32_t maxValue) {
    printf("%s",prompt);

    while (true) {
        char buff[12];
        bzero(buff,sizeof(buff));
        if (fgets(buff,sizeof(buff),stdin) == NULL) {
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

        *result = (__uint32_t)num;
        return true;
    }
}

bool getValidString(const char* prompt,char* result,int maxLength) {
    printf("%s",prompt);

    while (true) {
        if (fgets(result,maxLength+1,stdin) == NULL) {
            printf("输入错误，请重新输入\n");
            continue;
        }

        // 去除换行符
        char *newLine = strchr(result, '\n');
        if (newLine != NULL) {
            *newLine = '\0';
        }

        // 检查是否为空
        if (strlen(result)==0) {
            printf("输入不能为空，请重新输入\n");
            continue;
        }
        return true;
    }
}