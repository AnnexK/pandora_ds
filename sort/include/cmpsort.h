#ifndef DS_LAB6_CMPSORT
#define DS_LAB6_CMPSORT

#include "swap.h"

typedef int (*cmpfoo)(const void *, const void *);

unsigned int sort(void *base,
		  size_t n,
		  size_t size,
		  cmpfoo f);

#endif
