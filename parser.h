#ifndef PARSER_H
#define PARSER_H

#include "token.h"
#include "ast_tree.h"
#include "lexer.h"


typedef struct parser_s {
    lexer_t *lexer;
    token_t *current_token;
} parser_t;

parser_t *parser_new(lexer_t *lexer);

void parser_free(parser_t *parser);

void parser_error(parser_t *parser);

token_t *parser_eat(parser_t *parser, int token_type);

node_t *parser_factor(parser_t *parser);

node_t *parser_term(parser_t *parser);

node_t *parser_expr(parser_t *parser);

node_t *parser_parse(parser_t *parser);

#endif
