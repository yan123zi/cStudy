#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

const char* fileName = "files/test.txt";

void sendTestFile(int sock);

int main() {
    // 创建套接字
    int serv_sock = socket(AF_INET, SOCK_STREAM, 0);

    // 将套接字跟IP，端口绑定
    struct sockaddr_in serv_addr = {0};
    serv_addr.sin_family = AF_INET; // 使用ipv4地址
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // 具体ip地址
    serv_addr.sin_port = htons(8888); // 端口
    bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    // 进入监听状态，等待用户发起请求
    listen(serv_sock, 20);

    // 接收客户端请求
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_len = sizeof(clnt_addr);
    int clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_len);

    // 向客户端发送数据
    char str[] = "http://www.baidu.com";
    write(clnt_sock, str, strlen(str));
    sendTestFile(clnt_sock);

    // 关闭套接字
    close(clnt_sock);
    close(serv_sock);
    printf("server terminated\n");
    return 0;
}

void sendTestFile(int sock) {
    FILE *file = fopen(fileName, "rb");
    if (!file) {
        printf("Can't open file %s\n", fileName);
        return;
    }

    char buf[1024];
    while (fgets(buf, sizeof(buf), file)) {
        printf("%s", buf);
        send(sock, buf, strlen(buf), 0);
    }


    // printf("%s\n", buf);
    fclose(file);
}