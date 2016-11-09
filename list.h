#ifndef LIST_H
#define LIST_H

#include "ast_tree.h"

typedef struct list_node_s  {
	node_t *data;
	struct list_node_s* next;
	struct list_node_s* prev;
} list_node_t;

typedef struct list_s {
    list_node_t *head;
} list_t;

list_node_t* list_node_new(node_t *x);

list_t *list_new(void);

void list_prepend(list_t *list, node_t *x);

void list_append(list_t *list, node_t *x);

void list_remove_first(list_t *list);

void list_remove_last(list_t *list);

void list_free(list_t *list);

#endif
