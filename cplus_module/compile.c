//
// 分析汇编代码
//

int g(int x) {
    return x + 3;
}

int f(int x) {
    return g(x);
}

// gcc -S -o compile.s compilie.c -m64
// rsp : 相对基地址移动地址
// rbp : 相对基地址起始地址
// eax : 暂存一些数值
int main(void) {
    return f(8) + 1;
}
