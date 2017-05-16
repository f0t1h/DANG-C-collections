#ifndef __LLIST
#define __LLIST
#include "common.h"
typedef struct __llist_t llist_t;

llist_t *ll_init(size_t item_size);
void *ll_insert(llist_t *list);
void ll_remove(llist_t *list);
int ll_has_next(llist_t *list);
void *ll_next(llist_t *list);
int ll_has_prev(llist_t *list);
void *ll_prev(llist_t *list);

llist_t *ll_rep(llist_t *list);
void ll_rewind(llist_t *list);
void ll_reac(llist_t *list);
void ll_clear(llist_t *list);
void ll_free(llist_t *list);

#endif
