//
// Created by Administrator on 2024/5/31.
//

#include <iostream>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <cstdio>
#include <cstdlib>

#define MAX_EVENTS 2
#define BUFFER_SIZE 1024
#define SERVER_IP "101.33.215.110"
#define SERVER_PORT 12345

// 设置文件描述符为非阻塞模式
void set_nonblocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

int main() {
    int sock_fd, epfd;
    struct sockaddr_in server_addr;

    // 创建套接字
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd == -1) {
        perror("socket");
        exit(1);
    }

//    // 设置套接字为非阻塞
//    set_nonblocking(sock_fd);

    // 服务器地址
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    // 连接服务器
    if (connect(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect");
        close(sock_fd);
        exit(1);
    }

    // 创建 epoll 实例
    epfd = epoll_create1(0);
    if (epfd == -1) {
        perror("epoll_create1");
        close(sock_fd);
        exit(1);
    }

    struct epoll_event ev, events[MAX_EVENTS];

    // 添加标准输入到 epoll 实例
    ev.events = EPOLLIN;
    ev.data.fd = STDIN_FILENO;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, STDIN_FILENO, &ev) == -1) {
        perror("epoll_ctl: stdin");
        close(sock_fd);
        close(epfd);
        exit(1);
    }

    // 添加套接字到 epoll 实例
    ev.events = EPOLLIN | EPOLLOUT;
    ev.data.fd = sock_fd;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, sock_fd, &ev) == -1) {
        perror("epoll_ctl: sock_fd");
        close(sock_fd);
        close(epfd);
        exit(1);
    }

    while (true) {
        int nfds = epoll_wait(epfd, events, MAX_EVENTS, -1);
        if (nfds == -1) {
            perror("epoll_wait");
            close(sock_fd);
            close(epfd);
            exit(1);
        }

        for (int i = 0; i < nfds; ++i) {
            if (events[i].data.fd == STDIN_FILENO) {
                // 处理标准输入
                char buffer[BUFFER_SIZE];
                memset(buffer, 0, BUFFER_SIZE);
                if (fgets(buffer, BUFFER_SIZE, stdin) != NULL) {
                    // 发送数据到服务器
                    int len = strlen(buffer);
                    if (send(sock_fd, buffer, len, 0) == -1) {
                        perror("send");
                    }
                }
            } else if (events[i].data.fd == sock_fd) {
                // 处理服务器返回的数据
                if (events[i].events & EPOLLIN) {
                    char buffer[BUFFER_SIZE];
                    memset(buffer, 0, BUFFER_SIZE);
                    int n = recv(sock_fd, buffer, BUFFER_SIZE, 0);
                    if (n == -1) {
                        perror("recv");
                    } else if (n == 0) {
                        // 服务器关闭连接
                        std::cout << "Server closed connection" << std::endl;
                        close(sock_fd);
                        close(epfd);
                        exit(1);
                    } else {
                        std::cout << "Received from server: " << buffer << std::endl;
                    }
                }
            }
        }
    }

    close(sock_fd);
    close(epfd);
    return 0;
}
