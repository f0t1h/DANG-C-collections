#include "DANG/hashtable.h"



ht_iter_t *make_ht_iterator(hashtable_t *table){
	if(table==NULL){return NULL;}
	ht_iter_t *iter = getMem(sizeof(ht_iter_t));
	iter->ht = table;
	iter->bucket_no = 0;
	iter->index = 0;
	if(table->number_of_items > 0 && table->buckets[0]->size == 0 && ht_iter_has_next(iter)){
		ht_iter_next(iter);
	}	
	return iter;
}

ht_iter_t *ht_iter_copy(ht_iter_t *iter){
	ht_iter_t *new_iter = getMem(sizeof(ht_iter_t));
	memcpy(new_iter,iter,sizeof(ht_iter_t));
	return new_iter;
}

int ht_iter_has_next(ht_iter_t *iter){
	return iter->ht->size > iter->bucket_no;
}
int ht_iter_next(ht_iter_t *iter){
	size_t bucket_size = iter->ht->buckets[iter->bucket_no]->size;
	if(bucket_size <= iter->index+1){
		iter->index = 0;
		iter->bucket_no++;
		while( iter->ht->size > iter->bucket_no 
			&& iter->ht->buckets[iter->bucket_no]->size <= 0){
			iter->bucket_no++;
		}
	}
	else{
		iter->index++;
	}
	return iter->ht->size > iter->bucket_no;
}

void *ht_iter_get_key(ht_iter_t *iter){
	return ((pair_t *) vector_get(iter->ht->buckets[iter->bucket_no],iter->index))->key;
}

void *ht_iter_get_value(ht_iter_t *iter){
	return ((pair_t *) vector_get(iter->ht->buckets[iter->bucket_no],iter->index))->value;
}

void ht_iter_free(ht_iter_t *iter){
	freeMem(iter,sizeof(ht_iter_t));
}


size_t ht_default_hash_function(hashtable_t *table, const void *key){
	return *(int *)key % table->size;
}

void pair_free(hashtable_t *table, pair_t *pair){
	table->key_rmv(pair->key);
	table->val_rmv(pair->value);
}


hashtable_t *ht_init( size_t table_size,size_t key_size, size_t item_size){
	hashtable_t *new_table = getMem(sizeof(hashtable_t));
	new_table->size = table_size;
	new_table->buckets = getMem(sizeof(vector_t *) * table_size);
	
	int i;
	new_table->value_size = item_size;
	new_table->key_size = key_size;
	new_table->optimal_load_factor = OPTIMAL_LOAD_FACTOR;
	new_table->hf = &ht_default_hash_function;
	new_table->number_of_items = 0;

	new_table->key_rmv = &free;
	new_table->val_rmv = &free;
	for(i=0;i<table_size;i++){
		new_table->buckets[i] = vector_init(sizeof(pair_t),INIT_BUCKET_SIZE);
	}
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
	table->number_of_items++;
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
			table->key_rmv(tpair->key);
			table->val_rmv(tpair->value);
			vector_remove(bucket,i);
			table->number_of_items--;
			return;
		}
	}
}

vector_t *ht_to_vector(hashtable_t *table){
	int i,j;
	vector_t *set = vector_init(sizeof(pair_t),table->number_of_items);
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

	table->size = new_size;
	for(i=0;i<new_size;i++){
		table->buckets[i] =
			 vector_init(sizeof(pair_t),INIT_BUCKET_SIZE);
	} 

	pair_t *tpair;
	for(i=0;i<pairs->size;i++){
		tpair=vector_get(pairs,i);
		__ht_put_pair(table,tpair);
	}


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
			pair_free(table,vector_get(bucket,j));
		}
		vector_free(bucket);
	}
	free(table->buckets);
	free(table);
}

void ht_set_key_remove_function(hashtable_t *table, void (*rmv)(void*)){
	table->key_rmv = rmv;
}

void ht_set_val_remove_function(hashtable_t *table, void (*rmv)(void*)){
	table->val_rmv = rmv;
}

int ht_test(int argc, char **argv){
	hashtable_t *table = ht_init(4,sizeof(int),sizeof(int));
	int i;
	int *val;
	for(i=0;i<4;i++){
		int *tmp = malloc(sizeof(int));
		*tmp = (113555427*i)%15;
		val = ht_put(table,tmp);
		*val = *tmp;
		free(tmp);
	}
	ht_iter_t *iter = make_ht_iterator(table);
	ht_iter_t *iter2 = NULL;
	while(ht_iter_has_next(iter)){
		iter2=ht_iter_copy(iter);
		ht_iter_next(iter2);
		int *key = ht_iter_get_key(iter);
		int *value = ht_iter_get_value(iter);
		while(ht_iter_has_next(iter2)){
			int *key2 = ht_iter_get_key(iter2);
			int *value2 = ht_iter_get_value(iter2);

			printf("k:%d\tv:%d\tk2:%d\tv2:%d\n",*key,*value,*key2,*value2);
			ht_iter_next(iter2);
		}
		ht_iter_free(iter2);
		ht_iter_next(iter);
	}
	ht_iter_free(iter);
	ht_free(table);
	return 0;
}


