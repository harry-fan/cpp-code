#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 12345
#define BACKLOG 5
#define BUF_SIZE 200

int conn_amount;
int fd_A[BACKLOG];

void showclient() {
    int i;
    printf("client amount:%d\n", conn_amount);
    for (i = 0; i < BACKLOG; i++) {
        printf("%d %d\n", i, fd_A[i]);
    }
}

void start_server() {
    socklen_t sin_size;
    int sock_fd, new_fd;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;

    int yes = 1;
    int ret = 0, i = 0;
    char buf[BUF_SIZE];
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0) {
        perror("socket error!");
        exit(1);
    }

    if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
        perror("setsockopt error");
        exit(1);
    } 

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
    memset(server_addr.sin_zero, '\0', sizeof(server_addr.sin_zero));

    if (bind(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        exit(1);
    }

    if (listen(sock_fd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }
    printf("listen port %d\n", PORT);

    fd_set fdsr;
    int maxsock;
    struct timeval tv;
    conn_amount = 0;
    sin_size = sizeof(client_addr);
    maxsock = sock_fd;
    while (1) {
        FD_ZERO(&fdsr);
        FD_SET(sock_fd, &fdsr);
        tv.tv_sec = 30;
        tv.tv_usec = 0;

        for (i = 0; i < BACKLOG; i++) {
            if (fd_A[i] != 0) {
                FD_SET(fd_A[i], &fdsr);
            }
        }
        ret = select(maxsock + 1, &fdsr, NULL, NULL, &tv);
        if (ret < 0) {
            perror("select");
            exit(1);
        } else if (ret == 0) {
            printf("timeout\n");
            continue;
        }

        if (FD_ISSET(sock_fd, &fdsr)) {
            new_fd = accept(sock_fd, (struct sockaddr*)&client_addr, &sin_size);
            if (new_fd <=0) {
                perror("accept");
                continue;
            }
            if (conn_amount < BACKLOG) {
                fd_A[conn_amount++] = new_fd;
                printf("new connect client[%d] %s %d\n", conn_amount, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
                if (new_fd > maxsock) {
                    maxsock = new_fd;
                }
            } else {
                printf("max connect arrive\n");
                send(new_fd, "bye", 4, 0);
                close(new_fd);
                break;
            }

        }
        for (i = 0; i < BACKLOG; i++) {
            int fd = fd_A[i];
            if (!fd) {
                continue;
            }
            FD_SET(fd, &fdsr);
            if (FD_ISSET(fd, &fdsr)) {
                int readcount = read(fd, buf, BUFSIZ);
                if (readcount == 0) {
                    printf("client close i:%d\n", i);
                    fd_A[i] = 0;
                    close(fd);
                } else {
                    printf("server recv: %s\n", buf);
                    char tmp[BUF_SIZE] = "i am server recv:";
                    strcat(tmp, buf);
                    send(fd, tmp, sizeof(tmp), 0);
                }
            }
        }
    }
    for (i = 0; i < BACKLOG; i++) {
        if (fd_A[i] != 0) {
            close(fd_A[i]);
        }
    }
    exit(0);
}

int main() {
    start_server();
}

