#ifndef __VECTOR__
#define __VECTOR__
#include "common.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#define REMP_SORTED 0
#define REMP_FAST 1
#define REMP_LAZY 2
typedef struct __vector_t{
	size_t item_sizeof;
	size_t limit;
	size_t size;
	void **items;
	int REMOVE_POLICY;
} vector_t;

vector_t *vector_init(size_t item_sizeof, size_t initial_limit);
int vector_put(vector_t *vector, void* item);
int vector_remove(vector_t *vector, size_t index);
void *vector_get(vector_t *vector, size_t index); //Optional for index check
void *vector_peek(vector_t *vector);
int vector_free(vector_t *vector);
void vector_zip(vector_t *vector);
void vector_clear(vector_t *vector);
#endif
