#include <stdio.h>

void f(int r) {
  int x = r;
  int y = 1;
  printf("%d\n", x + y);
}

int main() {
    int a = 1;
    f(a);
    return 0;
}
