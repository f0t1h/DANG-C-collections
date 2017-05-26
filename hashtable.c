#include "DANG/hashtable.h"

size_t ht_default_hash_function(hashtable_t *table, void *key){
	return *(int *)key % table->size;
}

void pair_free(pair_t *pair){
	free(pair->key);
	free(pair->value);
}


hashtable_t *ht_init( size_t table_size,size_t key_size, size_t item_size){
	hashtable_t *new_table = getMem(sizeof(hashtable_t));
	new_table->size = table_size;
	new_table->buckets = getMem(sizeof(vector_t *) * table_size);
	
	int i;
	for(i=0;i<table_size;i++){
		new_table->buckets[i] = vector_init(sizeof(pair_t),INIT_BUCKET_SIZE);
	}
	new_table->value_size = item_size;
	new_table->key_size = key_size;
	new_table->optimal_load_factor = OPTIMAL_LOAD_FACTOR;
	new_table->hf = &ht_default_hash_function;
	new_table->number_of_items = 0;

	return new_table;
}

void ht_update_load_factor(hashtable_t *table, double load_factor){
	table->optimal_load_factor = load_factor;
}


pair_t *ht_get(hashtable_t *table, void *key){
	size_t bucket_index = table->hf(table,key);
	vector_t *bucket = table->buckets[bucket_index];
	int i;
	pair_t *tpair;
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

int ht_has_key(hashtable_t *table, void *key){
	return ht_get(table,key)!=NULL;
}

//For internal use
void __ht_put_pair(hashtable_t *table, pair_t *pair){
	size_t bucket_index = table->hf(table,pair->key);
	vector_t *bucket = table->buckets[bucket_index];
	int i;
	pair_t *tpair;
	for(i=0;i<bucket->size;i++){
		tpair = vector_get(bucket,i);
		if(memcmp(tpair->key,pair->key,table->key_size) == 0){
			memcpy(tpair->value,pair->value,table->value_size);
		}
	}
	vector_put(bucket, pair);

}

void *ht_put(hashtable_t *table, void *key){

	ht_load_factor_check(table);
	size_t bucket_index = table->hf(table,key);
	vector_t *bucket = table->buckets[bucket_index];
	int i;
	pair_t *tpair;
	for(i=0;i<bucket->size;i++){
		tpair = vector_get(bucket,i);

		if(memcmp(tpair->key,key,table->key_size) == 0){
			return tpair->value;
		}
	}

	void *new_key = getMem(table->key_size);
	void *new_val = getMem(table->value_size);
	memcpy(new_key,key,table->key_size);
	pair_t new_pair;
	new_pair.key = new_key;
	new_pair.value = new_val;
	vector_put(bucket, &new_pair);
	tpair = vector_tail(bucket);
	table->number_of_items++;
	return tpair->value;
}

void ht_remove(hashtable_t *table, void *key){
	size_t bucket_index = table->hf(table,key);
	vector_t *bucket = table->buckets[bucket_index];
	int i;
	pair_t *tpair;
	for(i=0;i<bucket->size;i++){
		tpair = vector_get(bucket,i);
		if(memcmp(tpair->key,key,table->key_size) == 0){
			free(tpair->key);
			free(tpair->value);
			vector_remove(bucket,i);
			table->number_of_items--;
			return;
		}
	}
}

vector_t *ht_to_vector(hashtable_t *table){
	int i,j;
	vector_t *set = vector_init(sizeof(pair_t),table->size);
	for(i=0;i<table->size;i++){
		vector_t *bucket = table->buckets[i];
		for(j=0;j<bucket->size;j++){
			vector_put(set,vector_get(bucket,j));
		}
	}
	return set;
}

void ht_expand(hashtable_t *table){
	vector_t *pairs = ht_to_vector(table);

	//Adjust lf to 0.5	
	size_t new_size = (table->number_of_items <<1);
	int i;
	for(i=0;i<table->size;i++){
		vector_free(table->buckets[i]);
	}

	resizeMem((void **)&(table->buckets),sizeof(vector_t*)*table->size,sizeof(vector_t*)*new_size);

	for(i=0;i<new_size;i++){
		table->buckets[i] =
			 vector_init(sizeof(pair_t),INIT_BUCKET_SIZE);
	} 

	pair_t *tpair;
	for(i=0;i<pairs->size;i++){
		tpair=vector_get(pairs,i);
		__ht_put_pair(table,tpair);
	}
	table->size = new_size;

	vector_free(pairs);
	//TODO Handle freeing, it is abit complicated, since we should avoid double freeing
}

void ht_shrink(hashtable_t *table){
	vector_t *pairs = ht_to_vector(table);
	size_t new_size = (table->number_of_items<<1);
	int i;
	for(i=0;i<table->size;i++){
		vector_tabularasa(table->buckets[i]);
	}
	for(i=new_size;i<table->size;i++){
		vector_free(table->buckets[i]);
	}
	resizeMem((void **)&table->buckets,sizeof(vector_t *)*table->size,sizeof(vector_t *)*new_size);
	pair_t *tpair;

	for(i=0;i<pairs->size;i++){
		tpair=vector_get(pairs,i);
		 __ht_put_pair(table,tpair);
	}
	table->size = new_size;
	vector_free(pairs);
}
void ht_load_factor_check(hashtable_t *table){
	double lf = table->number_of_items / (double) table->size;

	if( lf > table->optimal_load_factor+0.2){
		ht_expand(table);
	}
	else if (lf < table->optimal_load_factor-0.2){
//		Is Auto-shrinking really a good idea?
//		ht_shrink(table);
	}
}

void ht_free(hashtable_t *table){
	int i,j;
	for(i=0;i<table->size;i++){
		vector_t *bucket = table->buckets[i];
		for(j=0;j<bucket->size;j++){
			pair_free(vector_get(bucket,j));
		}
		vector_free(bucket);
	}
	free(table->buckets);
	free(table);
}
int test_ht(int argc, char **argv){
	hashtable_t *table = ht_init(4,sizeof(int),sizeof(int));
	int i;
	int *val;
	for(i=0;i<112;i++){
		int *tmp = malloc(sizeof(int));
		*tmp = i*123+11;
		val = ht_put(table,&i);
		*val = *tmp;
		free(tmp);
	}
	for(i=0;i<32;i+=2){
		ht_remove(table,&i);
	}
	int *tmp;
	for(i=0;i<112;i++){
		tmp = ht_get_value(table,&i);
		printf("%d\n",tmp!=NULL?*tmp:-1);
	}
	ht_free(table);
	return 0;
}

