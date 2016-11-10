#include<stdio.h>
#include<stdlib.h>
#include "list.h"
#include "ast_tree.h"

list_node_t* list_node_new(node_t *x) {
	list_node_t* newNode = (list_node_t*) calloc(1, sizeof(list_node_t));
	newNode->data = x;
	newNode->prev = NULL;
	newNode->next = NULL;
	return newNode;
}

list_t *list_new(void) {
    list_t *list = (list_t *) calloc(1, sizeof(list_t));
    list->head = NULL;
    return list;
}

void list_prepend(list_t *list, node_t *x) {
	list_node_t *newNode = list_node_new(x);
	if(list->head == NULL) {
		list->head = newNode;
		return;
	}
	newNode->next = list->head; 
    list->head->prev = newNode;
	list->head = newNode;
}

void list_append(list_t *list, node_t *x) {
	list_node_t* temp = list->head;
	list_node_t* newNode = list_node_new(x);
	if(list->head == NULL) {
		list->head = newNode;
		return;
	}
	while(temp->next != NULL) temp = temp->next;
	temp->next = newNode;
	newNode->prev = temp;
}

void list_remove_first(list_t *list) {
	if(list->head == NULL) {
		return;
	}
    list_node_t *temp = list->head;
	list->head = list->head->next;
    free(temp);
}

void list_remove_last(list_t *list) {
	list_node_t* temp = list->head;
	if(list->head == NULL) {
		return;
	}
	while(temp->next != NULL) temp = temp->next;
	if (temp->prev) {
        if(temp->prev->next) {
            temp->prev->next = NULL;
            free(temp);
        }
    }
}

void list_free(list_t *list) {
    list_node_t *curr = list->head;
    list_node_t *temp = NULL;
    while (curr) {
        temp = curr;
        curr = curr->next;
        free(temp);
    }
    free(list);
}

