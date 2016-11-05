#include <malloc.h>
#include "ast_tree.h"

node_t *binop_new(node_t *left, token_t *op, node_t *right) {
    node_t *node = (node_t *) calloc(1, sizeof(node_t));
    node->type = TYPE_BINOP;
    node->left = left;
    node->right = right;
    node->op = op;
    node->token = op;
    return node;
}

void binop_free(node_t *binop) {
    free(binop->token);
    free(binop);
}

node_t *num_new(token_t *token) {
    node_t *node = (node_t *) calloc(1, sizeof(node_t));
    node->type = TYPE_NUM;
    node->token = token;
    node->value = token->value;
    return node;
}

void num_free(node_t *num) {
    free(num->token);
    free(num);
}

node_t *unaryop_new(token_t *op, node_t *expr) {
    node_t *node = (node_t *) calloc(1, sizeof(node_t));
    node->type = TYPE_UNARYOP;
    node->token = op;
    node->op = op;
    node->right = expr;
    return node;
}

void unaryop_free(node_t *unaryop) {
    free(unaryop->token);
    free(unaryop);
}
