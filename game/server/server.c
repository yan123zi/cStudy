//
// Created by zjyan on 2025/8/13.
//
#include "server.h"

#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>

int main() {
#ifndef NDEBUG
    setbuf(stdout, 0);
#endif

    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == 0) {
        perror("socket failed!");
        return 1;
    }
    struct sockaddr_in server_address = {0};
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_address.sin_port = htons(1234);
    bind(server_socket, (struct sockaddr *) &server_address, sizeof(server_address));

    listen(server_socket, 10);
    struct sockaddr_in client_address;
    socklen_t client_address_length = sizeof(client_address);
    int cli_socket = accept(server_socket, (struct sockaddr *) &client_address, &client_address_length);
    if (cli_socket == 0) {
        perror("accept failed!");
        return 1;
    }

    pthread_t client;
    pthread_create(&client,NULL, processSocket, &cli_socket);
    pthread_join(client,NULL);
    close(server_socket);
    close(cli_socket);

    return 0;
}

void *processSocket(void *client_socket) {
    int cli_sock = *(int *) client_socket;
    char sizeNo[PACKET_HEADER_SIZE];
    while (true) {
        bzero(sizeNo, sizeof(sizeNo));
        long packSize = recv(cli_sock, &sizeNo, sizeof(sizeNo), 0);
        if (packSize == 0) {
            break;
        }

        PACKET_LEN length;
        MSGId msgNo;
        memcpy(&length, sizeNo, sizeof(PACKET_LEN));
        memcpy(&msgNo, sizeNo + sizeof(PACKET_LEN), sizeof(MSGId));
        printf("MessageFromClient len: %d, msgNo: %d\n", length, msgNo);

        unsigned char *body = malloc(length);
        if (body == NULL) {
            printf("malloc body failed");
            continue;
        }
        ssize_t bodySize = recv(cli_sock, body, length, 0);
        if (bodySize == 0) {
            printf("recv body failed");
            break;
        }
        if (bodySize < length) {
            printf("recv body size less then length, bodySize: %d, length: %d\n", (int) bodySize, length);
            free(body);
            continue;
        }
        struct Packet *packet = unmarshalDataToPacket(body, length, msgNo);
        if (packet == NULL) {
            printf("unmarshalDataToPacket failed");
            free(body);
            break;
        }
        int ret = handlePacket(cli_sock, packet);
        if (ret < 0) {
            printf("handlePacket failed");
            free(body);
            free(packet);
            break;
        }
        free(body);
        free(packet);
    }

    return NULL;
}

int handlePacket(const int socket, const struct Packet *packet) {
    printf("Handling Packet from socket %d\n", socket);
    if (packet == NULL) {
        printf("Packet is NULL");
        return -1;
    }
    switch (packet->msgNo) {
        case AddItem:
            printf("Adding Item\n");
            return addItem(packet->body, packet->length, socket);
        case RemoveItem:
            printf("Removing Item\n");
            break;
        case UpdateItem:
            printf("Updating Item\n");
            break;
        case GetItem:
            printf("Getting Item\n");
            break;
        default:
            printf("Unknown Message Type\n");
            return -1;
    }

    printf("msgId err!");
    return -1;
}

int addItem(unsigned char *body, const PACKET_LEN length, int socket) {
    if (length != sizeof(struct AddItem)) {
        printf("addItem length err!");
        return -1;
    }

    struct AddItem *addItemReq = (struct AddItem *) body;
    printf("addItem itemId: %d, count: %d, itemName: %s\n", addItemReq->item.itemId, addItemReq->item.itemNum,
           addItemReq->item.itemName);
    return 0;
}
