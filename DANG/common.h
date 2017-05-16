#ifndef __COMMON
#define __COMMON
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define freeMem(a,_) free((a))
#define getMem(a) malloc((a))
void resizeMem(void **ptr,size_t old_size, size_t new_size);


#endif
