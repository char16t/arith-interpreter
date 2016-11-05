#include <malloc.h>
#include "interpreter.h"

interpreter_t *interpreter_new(parser_t *parser) {
    interpreter_t *interpreter = (interpreter_t *) calloc(1, sizeof(interpreter_t));
    interpreter->parser = parser;
    return interpreter;
}

void interpreter_free(interpreter_t *interpreter) {
    free(interpreter);
}

int interpreter_visit_num(interpreter_t *interpreter, node_t *node);
int interpreter_visit_binop(interpreter_t *interpreter, node_t *node);
int interpreter_visit_unaryop(interpreter_t *interpreter, node_t *node);

int interpreter_visit(interpreter_t *interpreter, node_t *node) {
    int value;

    if (node->type == TYPE_BINOP) {
        value = interpreter_visit_binop(interpreter, node);
        binop_free(node);
    }
    
    else if (node->type == TYPE_NUM) {
        value = interpreter_visit_num(interpreter, node);
        num_free(node);
    }

    else if (node->type == TYPE_UNARYOP) {
        value = interpreter_visit_unaryop(interpreter, node);
        unaryop_free(node);
    }

    return value;
}

int interpreter_visit_num(interpreter_t *interpreter, node_t *node) {
    return node->value;
}

int interpreter_visit_unaryop(interpreter_t *interpreter, node_t *node) {
    if (node->op->type == T_PLUS) {
        return interpreter_visit(interpreter, node->right);
    }

    if (node->op->type == T_MINUS) {
        return (-1)*interpreter_visit(interpreter, node->right); 
    }
}

int interpreter_visit_binop(interpreter_t *interpreter, node_t *node) {
    if (node->op->type == T_PLUS) {
        return interpreter_visit(interpreter, node->left) + interpreter_visit(interpreter, node->right);
    }
    else if (node->op->type == T_MINUS) {
        return interpreter_visit(interpreter, node->left) - interpreter_visit(interpreter, node->right);
    }
    else if (node->op->type == T_MUL) {
        return interpreter_visit(interpreter, node->left) * interpreter_visit(interpreter, node->right);
    }
    else if (node->op->type == T_DIV) {
        return interpreter_visit(interpreter, node->left) / interpreter_visit(interpreter, node->right);
    }
}

int interpreter_interpret(interpreter_t *interpreter) {
    node_t *tree = parser_parse(interpreter->parser);
    return interpreter_visit(interpreter, tree);
}


