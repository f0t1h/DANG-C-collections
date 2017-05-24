#ifndef __HTABLE
#define __HTABLE
#include <string.h>
#include <stdlib.h>

#include "common.h"
#include "vector.h"

#define INIT_BUCKET_SIZE 4
#define UPPER_BOUND_LOAD_FACTOR 0.8
#define LOWER_BOUND_LOAD_FACTOR 0.1

typedef struct __pair_t{
	void *key;
	void *value;
} pair_t;
#define pair_vector_get(V,I) (pair_t *)vector_get((V),(I))

typedef struct __hashtable_t{
	vector_t **buckets;
	size_t size;
	size_t key_size;
	size_t value_size;
	size_t number_of_items;
	double upper_load_factor;
	double lower_load_factor;
	size_t (*hf)(void *key, size_t table_size, size_t key_size);
} hashtable_t;

hashtable_t *ht_init( size_t table_size, size_t item_size);
size_t ht_default_hash_function(hashtable_t *table, void *key);
void *ht_put(hashtable_t *table, void *key);
void ht_remove(hashtable_t *table, void *key);
pair_t *ht_get(hashtable_t *table, void *key);
void *ht_get_value(hashtable_t *table, void *key);
int ht_has_key(hashtable_t *table, void *key);
#endif
