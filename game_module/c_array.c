#include <stdio.h>

int main() {
    int a[5] = {1,2,3,4, 5};
    /* 575e00  575e00  575e00 */
    printf("%p, %p, %p\n", a, &a[0], &a);

    /* 575e14  575e00 */
    int *ptr = (int *)(&a + 1);
    printf("%p, %p\n", ptr, &a);

    /* 5 */
    printf("%d\n", (ptr - a));
}
