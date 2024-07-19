#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <vector>
#include <map>

#define PORT 12345
#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    fd_set readfds;
    int maxfd, client_fd;
    char read_buffer[BUFFER_SIZE];
    std::map<int, struct sockaddr_in* > client_addrs;

    // 创建套接字
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // 设置服务器地址
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // 绑定套接字
    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // 监听连接
    if (listen(sockfd, 5) < 0) {
        perror("listen");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    while (1) {
        // 初始化文件描述符集合
        FD_ZERO(&readfds);
        FD_SET(STDIN_FILENO, &readfds); // 监视标准输入
        FD_SET(sockfd, &readfds);       // 监视套接字
        maxfd = sockfd;

        // 调用 select 函数
        int activity = select(maxfd + 1, &readfds, NULL, NULL, NULL);
        if (activity < 0) {
            perror("select");
            close(sockfd);
            exit(1);
        }

        // 检查标准输入是否准备好
        if (FD_ISSET(STDIN_FILENO, &readfds)) {
            char buffer[1024];
            int bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer) - 1);
            if (bytes_read > 0) {
                buffer[bytes_read] = '\0';
                printf("标准输入: %s\n", buffer);
            }
        }

        for (auto iter = client_addrs.begin(); iter != client_addrs.end(); ++iter) {
            client_fd = iter->first;
            auto client_addr = iter->second;
            if (!client_fd || nullptr == client_addr) {
                continue;
            }
            if (FD_ISSET(sockfd, &readfds)) {
                // 检查是否是关闭连接的情况
                int valread = 0;
                if ((valread = read(client_fd, read_buffer, BUFFER_SIZE)) == 0) {
                    // 关闭套接字并在数组中设置为 0
                    printf("客户端断开连接，IP 是 : %s，端口是 : %d\n",
                           inet_ntoa(client_addr->sin_addr), ntohs(client_addr->sin_port));
                    close(client_fd);
                    iter = client_addrs.erase(iter);
                } else {
                    // 其他情况：处理从客户端接收的数据
                    read_buffer[valread] = '\0';
                    printf("客户端 %d 发送的数据: %s\n", client_fd, read_buffer);

                    // 发送数据到客户端
                    const char *mesage = "返回给客户端";
                    send(client_fd, mesage, strlen(mesage), 0);
                }
            }
        }

        // 检查套接字是否准备好
        if (FD_ISSET(sockfd, &readfds)) {
            int new_sockfd;
            struct sockaddr_in client_addr;
            socklen_t client_len = sizeof(client_addr);
            new_sockfd = accept(sockfd, (struct sockaddr*)&client_addr, &client_len);
            if (new_sockfd < 0) {
                perror("accept");
                close(sockfd);
                exit(EXIT_FAILURE);
            }
            printf("新连接: %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
            maxfd = new_sockfd;
            client_addrs[new_sockfd] = &client_addr;
        }
    }

    close(sockfd);
    return 0;
}

