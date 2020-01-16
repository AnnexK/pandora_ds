#ifndef DS_LAB5_FUNCTIONS
#define DS_LAB5_FUNCTIONS

#include <stddef.h>

size_t division(const void *i)
{
    static unsigned int divisor = 17;
    return *(unsigned int *)i % divisor;
}

#endif
