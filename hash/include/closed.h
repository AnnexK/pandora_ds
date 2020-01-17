#ifndef DS_LAB5_CLOSED_BASIC
#define DS_LAB5_CLOSED_BASIC

#include "hash.h"

#define NOT_IMPLEMENTED 255

dictionary *make_dict(size_t, hashf);
void dest_dict(dictionary *);
unsigned int get_last_tries(void);

#endif
