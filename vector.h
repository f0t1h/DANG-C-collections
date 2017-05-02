#ifndef __VECTOR__
#define __VECTOR__
#include "common.h"
typedef struct __vector_t{
	size_t item_sizeof;
	size_t limit;
	size_t size;
	void **items;
} vector_t;

vector_t *vector_init(size_t item_sizeof, size_t initial_limit);
int vector_put(vector_t *vector, void* item);
void *vector_get(vector_t *vector, size_t index); //Optional for index check
int vector_free(vector_t *vector);
void vector_zip(vector_t *vector);
void vector_clear(vector_t *vector);
#endif
