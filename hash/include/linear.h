#ifndef DS_LAB5_LINEAR
#define DS_LAB5_LINEAR

#include "hash.h"

#define NOT_IMPLEMENTED 255

dictionary *make_dict(size_t, hashf);
void dest_dict(dictionary *);
unsigned int get_last_tries(void);

#endif
