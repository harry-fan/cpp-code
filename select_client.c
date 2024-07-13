#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 12345
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    char input[BUFFER_SIZE];

    // 创建套接字
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        exit(1);
    }

    // 设置服务器地址
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("inet_pton");
        close(sockfd);
        exit(1);
    }

    // 连接到服务器
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("已连接到服务器 %s:%d\n", SERVER_IP, SERVER_PORT);

    while (1) {
        // 从外部获取输入数据
        printf("请输入发送到服务器的数据: ");
        if (fgets(input, BUFFER_SIZE, stdin) == NULL) {
            printf("读取输入错误\n");
            break;
        }

        // 去掉输入字符串末尾的换行符
        input[strcspn(input, "\n")] = '\0';

        // 发送数据到服务器
        if (send(sockfd, input, strlen(input), 0) < 0) {
            perror("send");
            close(sockfd);
            exit(EXIT_FAILURE);
        }

        // 接收服务器的响应
        int bytes_read = recv(sockfd, buffer, BUFFER_SIZE - 1, 0);
        if (bytes_read < 0) {
            perror("recv");
            close(sockfd);
            exit(EXIT_FAILURE);
        } else if (bytes_read == 0) {
            printf("服务器断开连接\n");
            break;
        }

        // 确保接收的消息是以 null 结尾的字符串
        buffer[bytes_read] = '\0';
        printf("收到服务器的消息: %s\n", buffer);
    }

    // 关闭套接字
    close(sockfd);
    return 0;
}
