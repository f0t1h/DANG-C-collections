#include "vector.h"

vector_t *vector_init(size_t item_sizeof, size_t initial_limit){
	vector_t *new_vector = (vector_t *) getMem( sizeof(vector_t));
	new_vector->item_sizeof = item_sizeof;
	new_vector->items = (void **)  getMem( sizeof(void *) * initial_limit);
	new_vector->limit = initial_limit;
	new_vector->size = 0;
	return new_vector;
}

int vector_put(vector_t *vector, void *item){
	if(vector->limit == vector->size){
		size_t new_limit = vector->limit +( vector->limit>>1);
		resizeMem((void **)&(vector->items),vector->limit * sizeof(void *),sizeof(void *) * new_limit);
		vector->limit = new_limit;
	}
	vector->items[vector->size] = getMem(vector->item_sizeof);

	memcpy( vector->items[vector->size],item,vector->item_sizeof);
	vector->size = vector->size + 1;
	return 0;
}

void vector_clear( vector_t *vector){
	vector->size = 0;
}

void vector_zip( vector_t *vector){
	resizeMem((void **)&(vector->items),vector->limit * sizeof(void *),vector->size * sizeof(void *));
}

void *vector_get( vector_t *vector, size_t index){
	if(vector->size <= index){
		fprintf(stderr,"Access out of index\n");
		return NULL;
	}
	return vector->items[index];
}

int vector_free( vector_t *vector){
	int i;
	for(i = 0; i< vector->size;i++){
		freeMem(vector->items[i],vector->item_sizeof);
	}
	freeMem(vector->items,vector->limit * sizeof(void *));
	freeMem(vector,sizeof(vector_t));
	return 0;
}
