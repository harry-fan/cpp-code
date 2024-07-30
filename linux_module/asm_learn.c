#include <stdio.h>
#include <time.h>

int main() {
//    int v1 = 10, v2 = 21, v3 = 0;
//    asm volatile (
//            "movl $0, %%eax\n\t"    // move 0 (v3) to eax, eax clear 0
//            "addl %1, %%eax\n\t"    // add v1 to eax
//            "addl %2, %%eax\n\t"    // add v2 to eax
//            "movl %%eax, %0\n\t"    // move eax to v3
//            : "=m" (v3)             // "=m" output v3, = for only write
//            : "c" (v1), "d" (v2)    // v1 put c :ecx  v2 put d : edx
//            );
//    printf("v1: %d, v2: %d, v3:%d\n", v1, v2, v3);

    // test use time
    time_t tt;
    struct tm *t;
    asm volatile (
            "mov $0, %%eax\n\t"     // eax clear 0
            "mov $0xd, %%eax\n\t"   // 0xd: 系统调用号，二进制 13 放入 eax 中
                                    // 系统调用号可以在linux-5.4源码的syscall_32.tbl中查看
            "int $0x80\n\t"         // 内核处理系统调用
            "mov %%eax, %0\n\t"     // 返回值通过 eax 返回放入 tt
            : "=m" (tt)
            ); // replace tt = time(NULL);

    t = localtime(&tt);
    printf("time:%d-%02d-%02d %02d:%02d:%02d\n", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
    return 0;
}
