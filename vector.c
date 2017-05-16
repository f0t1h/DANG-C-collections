#include "DANG/vector.h"
vector_t *vector_init(size_t item_sizeof, size_t initial_limit){
	vector_t *new_vector = (vector_t *) getMem( sizeof(vector_t));
	new_vector->item_sizeof = item_sizeof;
	new_vector->items = (void **)  getMem( sizeof(void *) * initial_limit);
	new_vector->limit = initial_limit;
	new_vector->size = 0;
	new_vector->REMOVE_POLICY = 0;
	return new_vector;
}

int vector_put(vector_t *vector, void *item){
	if(vector->limit == vector->size){
		size_t new_limit = vector->limit +( vector->limit>>1) + 1;
		resizeMem((void **)&(vector->items),vector->limit * sizeof(void *),sizeof(void *) * new_limit);
		vector->limit = new_limit;
	}
	vector->items[vector->size] = getMem(vector->item_sizeof);

	memcpy( vector->items[vector->size],item,vector->item_sizeof);
	vector->size = vector->size + 1;
	return 0;
}

void vector_update_remove_policy(vector_t *vector, int policy){
	vector->REMOVE_POLICY = policy;
}

int vector_contains(vector_t *vector, void *item){
	int i;
	for(i=0;i<vector->size;i++){
		if(memcmp(vector->items[i], item,vector->item_sizeof)){
			return i;
		}
	}
	return -1;
}

int vector_remove(vector_t *vector, size_t index){
	if(vector->items[index] == NULL || vector->size <= index){
		return -1;
	}
	vector->size--;
	switch(vector->REMOVE_POLICY){
	case REMP_SORTED:
		freeMem(vector->items[index],sizeof(vector->item_sizeof));
		int i;
		for(i=index;i<vector->size;i++){
			vector->items[i] = vector->items[i+1];
		}
	break;
	case REMP_FAST:
		freeMem(vector->items[index],sizeof(vector->item_sizeof));
		vector->items[index] = vector->items[vector->size];
	break;
	case REMP_LAZY:
		fprintf(stderr,"Not Implemented Yet!!\n");
		exit(-1);
	break;
	default:
		fprintf(stderr,"UNKNOWN POLICY %d\n", vector->REMOVE_POLICY);
		return -2;
	}
	return 0;
}

void vector_clear( vector_t *vector){
	vector->size = 0;
}

//NOT TESTED YET
void vector_zip( vector_t *vector){
	resizeMem((void **)&(vector->items),vector->limit * sizeof(void *),vector->size * sizeof(void *));
}

void *vector_get( vector_t *vector, size_t index){
	#ifdef __DEBUG__
	if(vector->size <= index){
		fprintf(stderr,"Access out of index\n");
		return NULL;
	}
	#endif
	return vector->items[index];
}

void *vector_tail(vector_t *vector){
	return vector->items[vector->size-1];
}

void *vector_head(vector_t *vector){
	return vector->items[0];
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

int main_test(int argc, char **argv){
	vector_t *vector = vector_init(sizeof(int),4);
	int i;
	for(i=0;i<10;i++){
		vector_put(vector,&i);
	}
	vector->REMOVE_POLICY = REMP_FAST;
	vector_remove(vector,5);
	int *element;
	for(i=0;i<vector->size;i++){
		element = vector_get(vector,i);		
		printf("%d\n",*element);
	}
	vector_free(vector);
	return 0;
}

