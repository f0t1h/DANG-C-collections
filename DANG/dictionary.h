#ifndef __DICTIONARY
#define __DICTIONARY
#include "vector.h"
#include <stdlib.h>
#include <string.h>
typedef struct __value_node{
	struct __value_node *next;
	void *value;
} value_node;

typedef struct __dictionary_t{
	vector_t *elements;
	size_t key_size;
	size_t value_size;
	size_t (*hashfunction)(void *key);
} dictionary_t;

dictionary_t *dict_init(size_t key_size, size_t value_size);
void dict_set_hashfunction(dictionary_t *dict, size_t (*hf)(void *key));

#endif
