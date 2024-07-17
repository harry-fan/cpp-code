//
// Created by Administrator on 2024/7/17.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <linux/unistd.h>
#include <pthread.h>

#define gettid() syscall(__NR_gettid)

// 内核线程

// 线程函数
void *thread_function(void *arg) {
    printf("Kernel thread ID: %ld\n", gettid());
    return NULL;
}

int main() {
    pthread_t thread;

    // 创建内核线程
    if (pthread_create(&thread, NULL, thread_function, NULL)) {
        fprintf(stderr, "Error creating thread\n");
        return 1;
    }

    // 等待线程结束
    if (pthread_join(thread, NULL)) {
        fprintf(stderr, "Error joining thread\n");
        return 2;
    }

    return 0;
}
