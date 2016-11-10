#include <malloc.h>
#include "ast_tree.h"

node_t *binop_new(node_t *left, token_t *op, node_t *right) {
    node_t *node = (node_t *) calloc(1, sizeof(node_t));
    node->type = TYPE_BINOP;
    node->data.binop = (binop_node_t *) calloc(1, sizeof(binop_node_t));
    node->data.binop->left = left;
    node->data.binop->right = right;
    node->data.binop->op = op;
    node->data.binop->token = op;
    return node;
}

void binop_free(node_t *binop) {
    free(binop->data.binop->token);
    free(binop);
}

node_t *num_new(token_t *token) {
    node_t *node = (node_t *) calloc(1, sizeof(node_t));
    node->type = TYPE_NUM;
    node->data.num = (num_node_t *) calloc(1, sizeof(num_node_t));
    node->data.num->token = token;
    node->data.num->value = token->value;
    return node;
}

void num_free(node_t *num) {
    free(num->data.num->token);
    free(num);
}

node_t *unaryop_new(token_t *op, node_t *expr) {
    node_t *node = (node_t *) calloc(1, sizeof(node_t));
    node->type = TYPE_UNARYOP;
    node->data.unaryop = (unaryop_node_t *) calloc(1, sizeof(unaryop_node_t));
    node->data.unaryop->token = op;
    node->data.unaryop->op = op;
    node->data.unaryop->expr = expr;
    return node;
}

void unaryop_free(node_t *unaryop) {
    free(unaryop->data.unaryop->token);
    free(unaryop);
}
