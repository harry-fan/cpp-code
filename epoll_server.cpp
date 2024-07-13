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
#include <map>

#define MAX_EVENTS 1024
#define PORT 12345
#define BUFF_SIZE 1024

void set_noblock(int fd) {
    // 设置非阻塞
    int flags = fcntl(fd, F_GETFL, 0);
    fcntl(fd, flags | O_NONBLOCK);
}

int main() {
    int listen_fd, epfd;
    struct sockaddr_in server_addr;

    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd < 0) {
        perror("scoket");
        exit(1);
    }
    // 设置非阻塞
    set_noblock(listen_fd);

    // 绑定地址
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
    int ret = bind(listen_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (ret == -1) {
        close(listen_fd);
        perror("bind");
        exit(1);
    }

    // 监听
    if (listen(listen_fd, SOMAXCONN) == -1) {
        perror("listen");
        close(listen_fd);
        exit(1);
    }

    // 创建epoll
    epfd = epoll_create1(0);
    if (epfd == -1) {
        close(listen_fd);
        perror("epoll_create");
        exit(1);
    }

    // 添加套接字到epoll
    struct epoll_event ev, events[MAX_EVENTS];
    ev.events = EPOLLIN;
    ev.data.fd = listen_fd;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, listen_fd, &ev) == -1) {
        perror("add listen_fd epoll");
        close(listen_fd);
        close(epfd);
        exit(1);
    }

    // 连接套接字构建
    std::map<int, std::string> clients;
    while (1) {
        int nfds = epoll_wait(epfd, events, MAX_EVENTS, -1);
        if (nfds == -1) {
            perror("epoll wait");
            close(listen_fd);
            close(epfd);
            exit(1);
        }

        for (int i = 0; i < nfds; ++i) {
            if (events[i].data.fd == listen_fd) {
                // 新的连接进入
                struct sockaddr_in client_addr;
                socklen_t client_len = sizeof(client_addr);
                int client_fd = accept(listen_fd, (struct sockaddr*)&client_addr, &client_len);
                if (client_fd == -1) {
                    perror("client accept");
                    continue;
                }
                // 非阻塞
                set_noblock(client_fd);

                // 添加到 epoll 中，继续监听后续事件
                ev.events = EPOLLIN | EPOLLET;
                ev.data.fd = client_fd;
                if (epoll_ctl(epfd, EPOLL_CTL_ADD, client_fd, &ev) == -1) {
                    perror("add epfd");
                    close(client_fd);
                    continue;
                }

                // 添加到客户端集合中 用来管理fd
                clients[client_fd] = "";
                std::cout << "new connection come:" << inet_ntoa(client_addr.sin_addr)
                << " port:" << ntohs(client_addr.sin_port) << std::endl;
            } else {
                // 已经连接的客户端消息处理
                char buffer[BUFF_SIZE];
                int client_fd = events[i].data.fd;
                int n = read(client_fd, buffer, BUFF_SIZE);
                if (n == -1) {
                    perror("read");
                    close(client_fd);
                    epoll_ctl(epfd, EPOLL_CTL_DEL, client_fd, NULL);
                    clients.erase(client_fd);
                } else if (n == 0) {
                    std::cout << "client close!" << std::endl;
                    close(client_fd);
                    epoll_ctl(epfd, EPOLL_CTL_DEL, client_fd, NULL);
                    clients.erase(client_fd);
                } else {
                    // 接收消息转发
                    buffer[n] = '\0';
                    std::string message(buffer);
                    std::cout << "recv message:" << client_fd << "|info:" << message << std::endl;
                    for (auto &cv : clients) {
                        write(cv.first, message.c_str(), strlen(message.c_str()));
                    }
                }
            }
        }
    }
    close(epfd);
    close(listen_fd);
    return 0;
}