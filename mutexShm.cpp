//
// Created by Administrator on 2024/7/17.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pthread.h>

// g++ --std=c++11 mutexShm.cpp -o mutexShm.a -pthread -lrt

// 共享内存结构体
struct shared_data {
    int counter;
    pthread_mutex_t mutex;
};

int main() {
    int shm_fd;
    void *ptr;
    struct shared_data *shared;

    // 创建共享内存对象
    shm_fd = shm_open("/myshm", O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        return 1;
    }

    // 调整共享内存对象的大小
    ftruncate(shm_fd, sizeof(struct shared_data));

    // 将共享内存对象映射到进程地址空间
    ptr = mmap(0, sizeof(struct shared_data), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    // 在映射的共享内存上创建互斥锁
    shared = (struct shared_data *)ptr;
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED); // 设置为进程共享

    pthread_mutex_init(&(shared->mutex), &attr);

    // 父进程写入共享内存数据
    if (fork() == 0) {
        // 子进程读取共享内存数据
        for (int i = 0; i < 10; ++i) {
            pthread_mutex_lock(&(shared->mutex)); // 加锁

            shared->counter++;
            printf("Child Process: Counter = %d\n", shared->counter);

            pthread_mutex_unlock(&(shared->mutex)); // 解锁
            sleep(1);
        }
    } else {
        // 父进程写入共享内存数据
        for (int i = 0; i < 10; ++i) {
            pthread_mutex_lock(&(shared->mutex)); // 加锁

            shared->counter++;
            printf("Parent Process: Counter = %d\n", shared->counter);

            pthread_mutex_unlock(&(shared->mutex)); // 解锁
            sleep(1);
        }
    }

    // 等待子进程结束
    wait();

    // 销毁互斥锁和解除映射
    pthread_mutex_destroy(&(shared->mutex));
    munmap(ptr, sizeof(struct shared_data));
    close(shm_fd);

    // 删除共享内存对象
    shm_unlink("/myshm");

    return 0;
}
