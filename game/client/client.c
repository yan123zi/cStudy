//
// Created by zjyan on 2025/8/17.
//


#include <stdbool.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "client.h"

#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>


int main()
{
#ifndef NDEBUG
    setbuf(stdout, 0);
#endif
    int cli_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (cli_socket < 0)
    {
        printf("Error creating socket\n");
        return 1;
    }

    struct sockaddr_in server = {0};
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(1234);
    int conn_status = connect(cli_socket, (struct sockaddr *)&server, sizeof(server));
    if (conn_status < 0)
    {
        printf("Error connecting to server\n");
        return 1;
    }

    pthread_t server_t;
    pthread_create(&server_t, NULL, processInput, &cli_socket);
    pthread_join(server_t, NULL);
    close(cli_socket);

    return 0;
}


void *processInput(void *socket)
{
    int cli_socket = *((int *)socket);
    char buffer[6];
    char close[] = "close";
    while (true)
    {
        bzero(buffer, sizeof(buffer));
        printf("请输入协议号：");
        if (fgets(buffer, sizeof(buffer),stdin) == NULL)
        {
            printf("Error reading from stdin\n");
            break;
        }

        if (strcmp(buffer, close) == 0)
        {
            break;
        }
        if (!isNumber(buffer))
        {
            printf("协议号格式错误\n");
            continue;
        }
        const MSGId msgNo = convertToLong(buffer);
        if (msgNo == -1)
        {
            printf("协议号格式错误\n");
            continue;
        }
        if (msgNo < MIN_MSG_NO || msgNo > MAX_MSG_NO)
        {
            printf("协议号错误\n");
            continue;
        }
        int ret = handleInput(msgNo, cli_socket);
        if (ret < 0)
        {
            printf("handleInput failed!");
            break;
        }
    }
}

int handleInput(MSGId msgNo, int socket)
{
    struct Packet *packet = NULL;
    switch (msgNo)
    {
    case AddItem:
        packet = handleAddItem();
        break;
    case RemoveItem:
        break;
    case UpdateItem:
        break;
    case GetItem:
        break;
    default:
        break;
    }

    if (packet == NULL)
    {
        return -1;
    }
    unsigned char *data = marshalPacketToData(packet);
    if (data == NULL)
    {
        return -1;
    }
    long packSize = send(socket, data, packet->length + PACKET_HEADER_SIZE, 0);
    if (packSize == 0)
    {
        printf("Error sending data to server\n");
        free(data);
        free(packet);
        return -1;
    }
    free(data);
    free(packet);
    return 0;
}

struct Packet *handleAddItem()
{
    struct Item *item = malloc(sizeof(struct Item));
    if (item == NULL)
    {
        perror("Error allocating memory for item");
        return NULL;
    }

    if (!getValidNumber("请输入道具Id：", &item->itemId, 1, 0xFFFFFFFF))
    {
        free(item);
        return NULL;
    }

    char nameBuffer[21];
    if (!getValidString("请输入道具名称：", nameBuffer, sizeof(item->itemName)))
    {
        free(item);
        return NULL;
    }
    strcpy(item->itemName, nameBuffer);

    if (!getValidNumber("请输入道具数量：", &item->itemNum, 0, 0xFFFFFFFF))
    {
        free(item);
        return NULL;
    }

    struct AddItem *addItem = malloc(sizeof(struct AddItem));
    if (addItem == NULL)
    {
        perror("Error allocating memory for addItem");
        free(item);
        return NULL;
    }

    memcpy(&addItem->item, item, sizeof(struct Item));
    free(item);
    struct Packet *packet = unmarshalDataToPacket((unsigned char *)addItem, sizeof(struct AddItem), AddItem);
    if (packet == NULL)
    {
        free(addItem);
        return NULL;
    }
    free(addItem);
    return packet;
}
