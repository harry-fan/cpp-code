//
// Created by Administrator on 2024/7/17.
//

// 用户态线程，多个用户态线程对应一个内核线程

#include <pthread.h>
#include <stdio.h>

void *print_message(void *ptr) {
    char *message;
    message = (char *) ptr;
    printf("%s \n", message);
    return NULL;
}

int main() {
    pthread_t thread1, thread2;
    const char *message1 = "Thread 1";
    const char *message2 = "Thread 2";

    // 创建线程
    pthread_create(&thread1, NULL, print_message, (void*) message1);
    pthread_create(&thread2, NULL, print_message, (void*) message2);

    // 等待线程结束
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    return 0;
}
