#include "DANG/graph.h"



graph_t *graph_init(size_t init_size, size_t node_size){
        graph_t *new_graph = ht_init(init_size, node_size, sizeof(vector_t*));
        return new_graph;
}

void graph_put_node(graph_t *g, void *item){
        vector_t **edges = ht_put(g,item);
        *edges = vector_init(sizeof(g->key_size),INIT_EDGE_COUNT);
}

void graph_put_edge(graph_t *g, void **i1, void **i2){

        vector_t **e1 = ht_get_value(g,i1);
        if(e1==NULL || *e1==NULL){
                fprintf(stderr,"NULL NODE e1\n");
        }


        if(vector_contains(*e1,*i2)==-1){
                vector_put(*e1,i2);
        }


        vector_t **e2 = ht_get_value(g,i2);

        if(e2==NULL || *e2==NULL){
                fprintf(stderr,"NULL NODE e2\n");
        }

        if(vector_contains(*e2,*i1)==-1){

                vector_put(*e2,i1);
        }
}

int main(int argc, char **argv){
	graph_t *g = graph_init(4,sizeof(int));
	int i=1;
	int j=2;
	graph_put_node(g,&i);
	graph_put_node(g,&j);
	graph_put_edge(g,&i,&j);
	vector_t *pairs = ht_to_vector(g);
	int k,t;
	pair_t *tmp;
	int *edge;
	for(k=0;k<pairs->size;k++){
		tmp = vector_get(pairs,k);
		printf("key = %d\n", *(int *)tmp->key);
		vector_t **edgep = tmp->value;
		vector_t *edges = *edgep;
		for(t=0;t<edges->size;t++){
			edge = vector_get(edges,t);
			printf("\tedge = %d\n",*edge);
		}		
	}

	return 0;
}
