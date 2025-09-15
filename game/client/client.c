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

int main() {
    int cli_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (cli_socket < 0) {
        printf("Error creating socket\n");
        return 1;
    }

    struct sockaddr_in server = {0};
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(1234);
    int conn_status=connect(cli_socket, (struct sockaddr *)&server, sizeof(server));
    if (conn_status < 0) {
        printf("Error connecting to server\n");
        return 1;
    }

    pthread_t server_t;
    pthread_create(&server_t, NULL,processInput ,&cli_socket);
    pthread_join(server_t, NULL);
    close(cli_socket);

    return 0;
}


void* processInput(void* socket) {
    int cli_socket = *((int*)socket);
    char buffer[1024];
    char close[]="close\n";
    while (true) {
        bzero(buffer, 1024);
        if (fgets(buffer,sizeof(buffer),stdin) == NULL) {
            printf("Error reading from stdin\n");
            break;
        }

        if (strcmp(buffer, close) == 0) {
            break;
        }
        ssize_t size = send(cli_socket, &buffer, strlen(buffer), 0);
        if (size < 0) {
            printf("Error writing to socket\n");
            break;
        }
        ssize_t recvSize=recv(cli_socket, &buffer, sizeof(buffer), 0);
        if (recvSize < 0) {
            printf("Error reading from socket\n");
            break;
        }
        printf("ReceiveMsgFromServer: %s", buffer);
    }
}