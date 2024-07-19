#include <iostream>
#include <cstring>
#include <sys/ipc.h>
#include <sys/shm.h>

#define BUFF_SIZE 1024  // 缓冲区大小
#define SHM_SIZE 1024   // 共享内存的大小
#define SHM_KEY 5678    // 共享内存的键值

int main() {
    // 创建共享内存段
    int shmid = shmget(SHM_KEY, SHM_SIZE, IPC_CREAT | 0666);
    if (shmid < 0) {
        perror("shmget");
        exit(1);
    }

    // 将共享内存映射到进程的地址空间
    char *shm = (char *)shmat(shmid, NULL, 0);
    if (shm == (char *) -1) {
        perror("shmat");
        exit(1);
    }

    while (true) {
        char buffer[BUFF_SIZE];
        std::cout << "请输入你想做什么:read/write?";
        std::cin.getline(buffer, BUFF_SIZE);
        if (strcmp(buffer, "write") == 0) {
            std::cout << "请输入你想塞入共享内存的数据:";
            std::cin.getline(buffer, BUFF_SIZE);
            size_t bufferSize = strlen(buffer) + 1;
            memcpy(shm, buffer, bufferSize);
            std::cout << "数据已塞入共享内存" << std::endl;
        } else if (strcmp(buffer, "read") == 0) {
            std::cout << "从共享内存获取的数据：" << shm << std::endl;
        } else {
            std::cout << "输入错误：" << buffer << std::endl;
        }
    }

    // 解除共享内存映射
    if (shmdt(shm) == -1) {
        perror("shmdt");
        exit(1);
    }

    return 0;
}
