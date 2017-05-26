#include "DANG/graph.h"



graph_t *graph_init(size_t init_size, size_t node_size){
        graph_t *new_graph = ht_init(init_size, node_size, sizeof(vector_t));
        return new_graph;
}

void graph_put_node(graph_t *g, void *item){
        vector_t *edges = ht_put(g,item);
//        edges = vector_init(sizeof(g->key_size),INIT_EDGE_COUNT);
	edges->item_sizeof = g->key_size;
	edges->items = (void **) getMem(sizeof(void *) * INIT_EDGE_COUNT);
	edges->limit = INIT_EDGE_COUNT;
	edges->size = 0;
	edges->REMOVE_POLICY = 0;
}

void graph_put_edge(graph_t *g, void *i1, void *i2){

        vector_t *e1 = ht_get_value(g,i1);
        if(e1==NULL){
                fprintf(stderr,"NULL NODE e1\n");
        }
	int c1= vector_contains(e1,i2);
	printf("%d-%d-c1=%d\n",*(int *)i1, *(int *)i2,c1);
//TODO: Fix contains do contains check
//        if(vector_contains(e1,i2)==-1){
         vector_put(e1,i2);
  //      }
/*
        vector_t *e2 = ht_get_value(g,i2);

        if(e2==NULL){
                fprintf(stderr,"NULL NODE e2\n");
        }
	int c2= vector_contains(e2,i1);
	printf("%d-%d-c1=%d\n",*(int *)i1, *(int *)i2,c2);
        
        if(vector_contains(e2,&i1)==-1){
                vector_put(e2,i1);
        }
*/
}

void graph_free(graph_t *g){
	int i,j,k;
	vector_t *bucket;
	pair_t *pair;
	vector_t *vect;
	for(i=0;i<g->size;i++){
		bucket = g->buckets[i];
		for(j=0;j<bucket->size;j++){
			pair = vector_get(bucket,j);
			vect = pair->value;
			for(k=0;k<vect->size;k++){
				free(vect->items[k]);
			}
			free(vect->items);
		}	
	}
	ht_free(g);
}


int main(int argc, char **argv){
	graph_t *g = graph_init(4,sizeof(int));

	int k,t;

	for(k=0;k<12;k++){
		graph_put_node(g,&k);
	}

	for(k=0;k<6;k++){
		int tmp = k +1;
		graph_put_edge(g,&k,&tmp);
		graph_put_edge(g,&tmp,&k);
	}
	vector_t *pairs = ht_to_vector(g);

	pair_t *tmp;
	int *edge;
	for(k=0;k<pairs->size;k++){
		tmp = vector_get(pairs,k);
		printf("key = %d\n", *(int *)tmp->key);
		vector_t *edges = tmp->value;

		for(t=0;t<edges->size;t++){
			edge = vector_get(edges,t);
			printf("\tedge = %d\n",*edge);
		}		
	}
	vector_free(pairs);
//	free(pairs->items);
//	free(pairs);
	graph_free(g);
	return 0;
}
