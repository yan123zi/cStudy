//
// Created by zjyan on 2025/8/11.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main() {
    // 创建套接字
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    // 向服务器发起请求
    struct sockaddr_in server = {0};
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(8888);
    connect(sock, (struct sockaddr *)&server, sizeof(server));

    // 读取服务器传回的数据
    char buff[1024] = {'\0'};
    recv(sock, buff, sizeof(buff), 0);
    printf("%s\n", buff);
    recv(sock, buff, sizeof(buff), 0);


    printf("Message from server: %s\n", buff);

    close(sock);
    return 0;
}
