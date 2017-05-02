#include "llist.h"

typedef struct __llist_node{
	void *data;
	struct __llist_node *prev;
	struct __llist_node *next;	
}llist_node;
struct __llist_t{
	llist_node *head;
	llist_node *cursor;
	size_t item_size;
};

llist_t *ll_init(size_t item_size){
	llist_t *list = malloc(sizeof(llist_t));
	list->head = malloc(sizeof(llist_node));
	list->head->next =NULL;
	list->head->prev =NULL;
	list->head->data =NULL;
	list->cursor = list->head;
	list->item_size = item_size;
	return list;
}

void *ll_insert(llist_t *list){
	llist_node *node = malloc(sizeof(llist_node));
	node->data = malloc(list->item_size);
	node->next = list->cursor->next;
	list->cursor->next = node;
	node->prev = list->cursor;
	list->cursor = node;
	return node->data;
}

void ll_remove(llist_t *list){
	if(list->cursor==NULL){
		return;
	}
	llist_node *tmp = list->cursor;
	if(list->cursor->prev !=NULL)
		list->cursor->prev->next = list->cursor->next;
	if(list->cursor->next !=NULL)
		list->cursor->next->prev = list->cursor->prev;
	list->cursor= list->cursor->prev;
	free(tmp->data);
	free(tmp);

}

int ll_has_next(llist_t *list){
	return list->cursor !=NULL && list->cursor->next !=NULL;
}

int ll_has_prev(llist_t *list){
	return list->cursor !=NULL && list->cursor->prev != NULL;
}
void *ll_next(llist_t *list){
	if(list->cursor == NULL || list->cursor->next ==NULL)
		return NULL;
	list->cursor = list->cursor->next;
	return list->cursor->data;	
}

void *ll_prev(llist_t *list){
	void *data = list->cursor->data;
	list->cursor = list->cursor->prev;
	return data;
}
llist_t *ll_rep(llist_t *list){
	llist_t *new_list = malloc(sizeof(llist_t));
	new_list->head=list->head;
	new_list->cursor=list->cursor;
	return new_list;
}

void ll_rewind(llist_t *list){
	list->cursor = list->head;
}
void ll_reac(llist_t *list){
	if( list->cursor == NULL){
		return;
	}
	llist_node *pointer = list->cursor->next;
	while(pointer!=NULL){
		list->cursor->next = pointer->next;
		free(pointer->data);//,list->item_size);
		free(pointer);
		pointer = list->cursor->next;
	}
}
void ll_clear(llist_t *list){
	ll_rewind(list);
	ll_reac(list);
	free(list->head);
}

void ll_free(llist_t *list){
	free(list);
}
