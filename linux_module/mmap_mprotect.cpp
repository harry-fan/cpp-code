#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main() {
    // 打开一个文件
    int fd = open("testfile", O_RDWR | O_CREAT, 0666);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // 扩展文件到 1MB
    if (ftruncate(fd, 1024 * 1024) == -1) {
        perror("ftruncate");
        exit(EXIT_FAILURE);
    }

    // 将文件映射到内存
    void *addr = mmap(NULL, 1024 * 1024, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }
    printf("addr info:%p\n", addr); // 0x7fd700d04000
    // 从下面可以看到 文件映射的内存是直接挂在到对应的文件上的
    // 并且这个大小是 1024 * 1024 没有额外分配的内存空间
    // 7fd700d04000-7fd700e04000 r--s 00000000 00:26 110   /mnt/hgfs/formyself/github/cpp-code/linux_module/testfile

    // 从内存写入文件
    strcpy((char *) addr, "this is a test message! for write to file!\n");

    // 将内容同步进文件
    if (msync(addr, 1024*1024, MS_SYNC) == -1) {
        perror("msync file error!");
        exit(EXIT_FAILURE);
    }

    // 修改映射内存的保护
    if (mprotect(addr, 1024 * 1024, PROT_READ) == -1) {
        perror("mprotect");
        exit(EXIT_FAILURE);
    }

    // 查看运行的 pid
    printf("cur mmap ppid:%d\n", getpid());
    getchar();

    // 解除内存映射
    if (munmap(addr, 1024 * 1024) == -1) {
        perror("munmap");
        exit(EXIT_FAILURE);
    }

    // 关闭文件
    close(fd);

    return 0;
}
