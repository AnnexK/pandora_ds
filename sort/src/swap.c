#include "swap.h"

void swap(void *a, void *b, size_t bufsiz)
{
    char buf[bufsiz];
    memcpy(buf, a, bufsiz);
    memcpy(a, b, bufsiz);
    memcpy(b, buf, bufsiz);
}
