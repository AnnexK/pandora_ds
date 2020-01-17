#ifndef DS_LAB5_DOUBLEHASH
#define DS_LAB5_DOUBLEHASH

#include "hash.h"

#define NOT_IMPLEMENTED 255

dictionary *make_dict(size_t, hashf, hashf);
void dest_dict(dictionary *);
unsigned int get_last_tries(void);

#endif
