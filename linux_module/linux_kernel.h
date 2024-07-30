//
// Created by Administrator on 2024/7/29.
//

#ifndef CPP_CODE_LINUX_KERNEL_H
#define CPP_CODE_LINUX_KERNEL_H

#define MAX_TASK_NUM 5
#define KERNEL_STACK_SIZE 1024*8

/* CPU state of this task */
struct Thread {
    unsigned long ip;   // eip
    unsigned long sp;   // esp
};

typedef struct PCB {
    int pid;             // progress id
    volatile long state; // progress state
    char stack[KERNEL_STACK_SIZE];  // heap stack
    struct Thread thread;
    unsigned long task_entry;   // start
    struct PCB *next;   // link
} tPCB;


#endif //CPP_CODE_LINUX_KERNEL_H
