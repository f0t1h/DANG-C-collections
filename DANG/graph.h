#ifndef __GRAPH
#define __GRAPH
#include "common.h"
#include "vector.h"
#include "hashtable.h"

#define INIT_EDGE_COUNT 8

typedef hashtable_t graph_t;

graph_t *graph_init(size_t init_size, size_t node_size);
void graph_put_node(graph_t *g, void *item);
//void graph_put_edge(graph_t *g, void *i1, void *i2);
#endif
