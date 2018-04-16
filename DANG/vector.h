#ifndef __VECTOR__
#define __VECTOR__
/*TODO:
 *	rename size if we want lazy removal
 *	NULL check for removal
 *	NULL init array
 *	insert and defragment functions
 */
#include "common.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#define REMP_SORTED 0
#define REMP_FAST 1
#define REMP_LAZY 2
typedef struct __vector_t{
	void **items;
	size_t item_sizeof;
	size_t limit;
	size_t size;
	int REMOVE_POLICY;
	int fragmental;
	void (*rmv)(void *);
} vector_t;
vector_t *vector_execute_for_all_and_save(vector_t *v, void *(*foo)(void *));
void vector_execute_for_all(vector_t *v, void (*foo)(void *));
typedef void (*foo_gen_no_ret)(void *);
typedef void *(*foo_gen_wi_ret)(void *);
#define vector_exec(V,FOO) _Generic((FOO), \
                foo_gen_no_ret:vector_execute_for_all,\
                foo_gen_wi_ret:vector_execute_for_all_and_save\
                        )((V),(FOO))
#define VECTOR_VARIABLE_SIZE 0

void vector_filter(vector_t *,int (*check)(void *));
vector_t *vector_select(vector_t *,int (*check)(void *));


vector_t *vector_init(size_t item_sizeof, size_t initial_limit);
int vector_put(vector_t *vector, void* item);
void vector_soft_put(vector_t *vector, void *item);
int vector_remove(vector_t *vector, size_t index);
int vector_contains(vector_t *vector, void *item);
int vector_comptains(vector_t *vector, void *item, int (*cmp)(const void*,const void*));
void *vector_get(vector_t *vector, size_t index); //Optional for index check
void *vector_head(vector_t *vector);
void *vector_tail(vector_t *vector);
void vector_insert(vector_t *vector, void *item, size_t index);
int vector_defragment(vector_t *vector);
void vector_free(void /*vector_t*/ *vector);
void vector_zip(vector_t *vector);
void vector_clear(vector_t *vector);
void vector_tabularasa(vector_t *vector);
void vector_set_remove_function(vector_t *vector, void (*rmv)(void*));


#endif
