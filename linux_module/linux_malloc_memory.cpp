//
// Created by Administrator on 2024/7/19.
//

#include <stdio.h>
#include <malloc.h>
#include <unistd.h>

// 在 x86_64 位 linux 上测试结果如下
void testBigMemoryMalloc() {
    printf("查看分配内存 %d\n", getpid());

    void *addr = malloc(2*128*1024);
    printf("申请内存的起始地址: %p\n", addr); // 0x7f33b752b010
    // 7f33b752b000-7f33b7571000 rw-p 00000000 00:00 0 在文件映射区域分配内存
    // 分配了 280kB 个字节。头部额外多分配了 16 个字节 尾部多分配了 279-256=23KB

    printf("查看分配内存 %d\n", getpid());
    getchar();

    free(addr);
    // 仍然可以看到之前分配的内存，并没有释放，为了内存复用
    printf("释放了256KB字节的内存\n");

    getchar();
}

void testMaxMemoryMalloc() {
    printf("查看分配内存 %d\n", getpid());

    void *addr = malloc(128*1024);
    printf("申请内存的起始地址: %p\n", addr); // 0x2200c20
    // 021ef000-02221000 rw-p 00000000 00:00 0  [heap] 在堆上分配内存
    // 分配了 200k 个字节。头部额外多分配了 72736 个字节

    printf("查看分配内存 %d\n", getpid());
    getchar();

    free(addr);
    // 仍然可以看到之前分配的内存，并没有释放，为了内存复用
    printf("释放了128KB字节的内存\n");

    getchar();
}

void testMinMemoryMalloc() {
    printf("查看分配内存 %d\n", getpid());

    void *addr = malloc(1);
    printf("申请内存的起始地址: %p\n", addr); // 0x13eec20
    // 013dd000 - 0140f000 rw-p 00000000 00:00 0  [heap] 在堆上分配内存
    // 分配了 200k 个字节。头部额外多分配了 72736 个字节

    printf("查看分配内存 %d\n", getpid());
    getchar();

    free(addr);
    // 仍然可以看到之前分配的内存，并没有释放，为了内存复用
    printf("释放了1字节的内存\n");

    getchar();
}

int main() {
    // 测试申请小内存，查看内存分配的信息
    testMinMemoryMalloc();

    // 测试申请大内存，查看内存分配信息
    testMaxMemoryMalloc();

    // 测试超大内存申请，查看分配内存信息
    testBigMemoryMalloc();

    // 从一个资料上看128k是进行分配堆和分配文件映射区的分割点。
    // 其实是针对不同的系统，64位系统就是256k是分割点。
    // 并且可以看到在堆上分配的内存，释放之后不会直接销毁
    // 从文件映射区分配的大内存，在free之后直接就还给了系统。
    return 0; // 进程结束回收分配的资源
}