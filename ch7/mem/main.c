#include <stdio.h>
#include "mem.h"

int main(int argc, char *argv[])
{
    int *ptr = (int *)malloc(100);
    *ptr = 42;
    printf("ptr: %10p\nblock: %lu\nvalue: %d\n\n", (void *)ptr, ((size_t *)ptr)[-1], *ptr);
    free(ptr);

    ptr = (int *)malloc(sizeof(int));
    *ptr = 69;
    printf("ptr: %10p\nblock: %lu\nvalue: %d\n\n", (void *)ptr, ((size_t *)ptr)[-1], *ptr);

    ptr = (int *)malloc(sizeof(int));
    *ptr = 123;
    printf("ptr: %10p\nblock: %lu\nvalue: %d\n\n", (void *)ptr, ((size_t *)ptr)[-1], *ptr);

    return 0;
}
