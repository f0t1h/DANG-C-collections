#include "DANG/hashtable.h"

hashtable_t *hashtable_init( size_t table_size,size_t key_size, size_t item_size){
	hashtable_t *new_table = getMem(sizeof(hashtable_t));
	new_table->size = table_size;
	new_table->buckets = getMem(sizeof(vector_t *) * table_size);
	
	int i;
	for(i=0;i<table_size;i++){
		new_table->table[i] = vector_init(INIT_BUCKET_SIZE,
			sizeof(pair_t));
	}
	new_table->value_size = item_size;
	new_table->key_size = key_size;
	new_table->allowed_load_factor = DEFAULT_LOAD_FACTOR;
	new_table->hf = &hashtable_default_hash_function;
	new_table->number_of_items = 0;

	return new_table;
}

void hashtable_update_load_factor(hashtable_t *table, double load_factor){
	table->allowed_load_factor = load_factor;
}

size_t hashtable_default_hash_function(hashtable_t *table, void *key){
	return *(size_t *)key % table_size;
}

pair_t *ht_get(hashtable_t *table, void *key){
	size_t bucket_index = table->hf(table,key);
	vector_t *bucket = table->buckets[bucket_index];
	int i;
	pait_t *tpair;
	for(i=0;i<bucket->size;i++){
		tpair = vector_get(bucket,i);
		if(memcmp(tpair->key,key,table->key_size) == 0){
			return tpair;
		}
	}
	return NULL;
}

void *ht_get_value(hashtable_t *table, void *key){
	pair_t *p = ht_get(table,key);
	if(p!=NULL){
		return p->value;
	}
	return NULL;
}

int ht_has_key(hashtable *table, void *key){
	return ht_get(table,key)!=NULL;
}

void *ht_put(hashtable_t *table, void *key){
	size_t bucket_index = table->hf(table,key);
	vector_t *bucket = table->buckets[bucket_index];
	int i;
	pait_t *tpair;
	for(i=0;i<bucket->size;i++){
		tpair = vector_get(bucket,i);
		if(memcmp(tpair->key,key,table->key_size) == 0){
			return tpair->value;
		}
	}
	vector_put(bucket, &(pair_t){key,0});
	tpair = vector_tail(bucket);
	table->number_of_items++;
	ht_load_factor_check(hashtable_t *table);
	return tpair->value;
}

void ht_remove(hashtable_t *table, void *key){
	size_t bucket_index = table->hf(table,key);
	vector_t *bucket = table->buckets[bucket_index];
	int i;
	pait_t *tpair;
	for(i=0;i<bucket->size;i++){
		tpair = vector_get(bucket,i);
		if(memcmp(tpair->key,key,table->key_size) == 0){
			vector_remove(bucket,i);
			return;
		}
	}
	table->number_of_items--;
	ht_load_factor_check(table);
}

vector_t *ht_to_vector(hashtable_t *table){
	int i,j;
	vector_t *set = vector_init(sizeof(pair_t),table->size);
	for(i=0;i<table->size;i++){
		vector_t *bucket = table->buckets[i];
		for(j=0;j<bucket->size;i++){
			vector_soft_put(set,vector_get(bucket,j));
		}
	}
	return set;
}

void ht_expand(hashtable_t *table){
	vector_t *pairs = ht_to_vector(table);
	size_t new_size = table->size + (table->size >>1) +1;
	resizeMem(&table->buckets,table->size,new_size);
	int i;
	for(i=0;i<table->size;i++){
		vector_tabularasa(table->buckets[i]);
	}
	for(i=table->size;i<new_size;i++){
		table->buckets[i] =
			 vector_init(INIT_BUCKET_SIZE,sizeof(pair_t));
	}
	for(i=0;i<pairs->size;i++){
		ht_put(table,pairs[i]);
	}
	freeMem(pairs);
	//TODO Handle freeing, it is abit complicated, since we should avoid double freeing
}

void ht_shrink(hashtable_t *table){
	
}
void ht_load_factor_check(hashtable_t *table){
	double lf = table->number_of_items / (double) table->size;
	if( lf > table->upper_load_factor){
		ht_expand(table);
	}
	else if (lf < table->lower_load_factor){
		ht_shrink(table);
	}
}

int main(int argc, char **argv){
	return 0;
}

