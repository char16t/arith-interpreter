#ifndef AST_TREE_H
#define AST_TREE_H

#define TYPE_BINOP   0
#define TYPE_NUM     1
#define TYPE_UNARYOP 2

#include "token.h"

struct binop_node_s;
struct num_node_s;
struct unaryop_node_s;

struct node_s {
    char type; /* 0 - binop; 1 - num; 2 - unaryop */
    /*
    struct node_s  *left;
    struct node_s  *right;
    token_t *token;
    token_t *op;
    token_value_t value;
    */
    union {
        struct binop_node_s *binop;
        struct unaryop_node_s *unaryop;
        struct num_node_s *num;
    } data;
};
typedef struct node_s node_t;

struct binop_node_s {
    node_t *left;
    node_t *right;
    token_t *op;
    token_t *token;
};
typedef struct binop_node_s binop_node_t;

struct num_node_s {
    token_t *token;
    token_value_t value;
};
typedef struct num_node_s num_node_t;

struct unaryop_node_s {
    token_t *token;
    token_t *op;
    node_t  *expr;
};
typedef struct unaryop_node_s unaryop_node_t;

node_t *binop_new(node_t *left, token_t *op, node_t *right);

void binop_free(node_t *binop);

node_t *num_new(token_t *token);

void num_free(node_t *num);

node_t *unaryop_new(token_t *op, node_t *expr);

void unaryop_free(node_t *unaryop);

#endif
