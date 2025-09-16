//
// Created by zjyan on 2025/8/13.
//
#include "server.h"

#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include <stdbool.h>
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
    struct sockaddr_in server_address={0};
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_address.sin_port = htons(1234);
    bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address));

    listen(server_socket, 10);
    struct sockaddr_in client_address;
    socklen_t client_address_length = sizeof(client_address);
    int cli_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_address_length);
    if (cli_socket == 0) {
        perror("accept failed!");
        return 1;
    }

    pthread_t client;
    pthread_create(&client,NULL,processSocket,&cli_socket);
    pthread_join(client,NULL);
    close(server_socket);
    close(cli_socket);

    return 0;
}

void* processSocket(void* client_socket) {
    int cli_sock = *(int*)client_socket;
    char sizeNo[1024];
    while (true) {
        bzero(sizeNo, sizeof(sizeNo));
        long packSize= recv(cli_sock,&sizeNo,sizeof(sizeNo),0);
        if (packSize == 0) {
            break;
        }
        printf("MessageFromClient len: %s",sizeNo);

    }

}
