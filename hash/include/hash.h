#ifndef DS_LAB5_DICT
#define DS_LAB5_DICT

#include <stddef.h>
#include <stdarg.h>
#include "pair.h"

#define SUCCESS 0
#define NOT_FOUND 1

typedef size_t (*hashf)(const void *);

struct _dict;

typedef struct _dict dictionary;

int insert(dictionary *, K, V);
pair *search(dictionary *, K);
int delete(dictionary *, K);
void print(dictionary *);

#endif
