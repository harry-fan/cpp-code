
#include <memory>
#include "linux_kernel.h"

tPCB task[MAX_TASK_NUM];
tPCB *my_current_task = NULL;
volatile int my_need_sched = 0;

void my_process(void);
void my_start_kernel(void) {
    int pid = 0;
    int i;

    task[pid].pid = pid;
    task[pid].state = 0;
    task[pid].task_entry = task[pid].thread.ip = (unsigned long)my_process; // 入口为 eip
    task[pid].thread.sp = (unsigned long)&task[pid].stack[KERNEL_STACK_SIZE - 1];
    task[pid].next = &task[pid];

    for (i = 0; i < MAX_TASK_NUM; i++) {
        memcpy(&task[i], &task[0], sizeof(tPCB));   // 拷贝0号进程的数据
        task[i].pid = i;
        task[i].state = -1;
        task[i].thread.sp = (unsigned long)&task[i].stack[KERNEL_STACK_SIZE - 1];
        task[i].next = task[i - 1].next;
        task[i - 1].next = &task[i];
    }

    /* start process 0 */
    pid = 0;
    my_current_task = &task[pid];
    asm volatile (
            "movl %1, %%esp\n\t"    // 将 task[pid].thread.sp 放入 esp
            "pushl %1\n\t"          // push ebp
            "pushl %0\n\t"          // push task[pid].thread.ip
            "ret\n\t"               // pop task[pid].thread.ip to eip
            "popl %%ebp\n\t"        // pop ebp 开始运行 0 号进程
            :
            : "c" (task[pid].thread.ip), "d" (task[pid].thread.sp)
            );
}

void my_process(void) {
    if (my_need_sched == 1) {
        my_need_sched = 0;
        printk(KERN_NOTICE, "xxxxxxx");
    }
}

void my_schedule(void) {
    tPCB *next;
    tPCB *prev;

    if (my_current_task == NULL || my_current_task->next == NULL) {
        return;
    }

    printk(KERN_NOTICE, "my kernel");

    next = my_current_task->next;
    prev = my_current_task;
    if (next->state == 0) { /* -1 unrunable 0 run >0 stoped*/
        // 进程上下文切换代码
        asm volatile (
                "pushl %%ebp\n\t"       // save ebp
                "movl %%esp,%0\n\t"     // save esp
                "movl %2,%%esp\n\t"     // restore esp
                "movl $1f, %1\n\t"      // save eip
                "pushl %3\n\t"          //
                "ret\n\t"               // restore eip
                "1:\t"                  // next process start
                "popl %%ebp\n\t"        //
                : "=m" (prev->thread.sp), "=m" (prev->thread.ip)
                : "m" (next->thread.sp), "m" (next->thread.ip)
                );
        my_current_task = next;
        printk(KERN_NOTICE, "switch process %d to %d", prev->pid, next->pid);
    }
}