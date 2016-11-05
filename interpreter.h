#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "parser.h"

typedef struct interpreter_s {
    parser_t *parser;
} interpreter_t;

interpreter_t *interpreter_new(parser_t *parser);

void interpreter_free(interpreter_t *interpreter);

int interpreter_visit(interpreter_t *interpreter, node_t *node);

int interpreter_visit_num(interpreter_t *interpreter, node_t *node);

int interpreter_visit_unaryop(interpreter_t *interpreter, node_t *node);

int interpreter_visit_binop(interpreter_t *interpreter, node_t *node);

int interpreter_interpret(interpreter_t *interpreter);

#endif
