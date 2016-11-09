#ifndef AST_TREE_H
#define AST_TREE_H

#define TYPE_BINOP   0
#define TYPE_NUM     1
#define TYPE_UNARYOP 2

#include "token.h"

typedef struct node_s {
    char type; /* 0 - binop; 1 - num; 2 - unaryop */
    struct node_s  *left;
    struct node_s  *right;
    token_t *token;
    token_t *op;
    token_value_t value;
} node_t;

node_t *binop_new(node_t *left, token_t *op, node_t *right);

void binop_free(node_t *binop);

node_t *num_new(token_t *token);

void num_free(node_t *num);

node_t *unaryop_new(token_t *op, node_t *expr);

void unaryop_free(node_t *unaryop);

#endif
