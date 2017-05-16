#include "DANG/common.h"

void resizeMem( void **ptr, size_t old_size, size_t new_size){
        void *ret;
        ret = realloc(*ptr,new_size);
        if( ret == NULL){
                fprintf(stderr,"Can't Realloc\n");
                exit(-1);
        }
        *ptr =ret;
}
