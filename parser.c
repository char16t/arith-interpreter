#include <malloc.h>
#include <stdlib.h>
#include "parser.h"


parser_t *parser_new(lexer_t *lexer) {
    parser_t *parser = (parser_t *) calloc(1, sizeof(parser_t));
    parser->lexer = lexer;
    parser->current_token = lexer_next_token(parser->lexer);
    return parser;
}

void parser_free(parser_t *parser) {
    free(parser->current_token->value.c);
    free(parser->current_token);
    free(parser);
}

void parser_error(parser_t *parser) {
    puts("Invalid token\n");
    exit(1);
}

token_t *parser_eat(parser_t *parser, int token_type) {
    if (parser->current_token->type == token_type) {
        token_t *prev = parser->current_token;
        parser->current_token = lexer_next_token(parser->lexer);
        return prev;
    } else {
        parser_error(parser);
    }
}

node_t *parser_expr(parser_t *parser);

node_t *parser_factor(parser_t *parser) {
    token_t *token = parser->current_token;

    if (token->type == T_PLUS) {
        token_t *plus = parser_eat(parser, T_PLUS);
        free(plus->value.c);
        return unaryop_new(token, parser_factor(parser));
    }

    else if (token->type == T_MINUS) {
        token_t *minus = parser_eat(parser, T_MINUS);
        free(minus->value.c);
        return unaryop_new(token, parser_factor(parser));
    }

    else if (token->type == T_INTEGER) {
        parser_eat(parser, T_INTEGER);
        return num_new(token);
    }
    else if (token->type == T_LPAREN) {
        token_t *lp = parser_eat(parser, T_LPAREN);
        node_t *node = parser_expr(parser);
        token_t *rp = parser_eat(parser, T_RPAREN);
        free(lp); free(rp);
        return node;
    }
}

node_t *parser_term(parser_t *parser) {
    token_t *token;

    node_t *node = parser_factor(parser);
    while (parser->current_token->type == T_MUL || parser->current_token->type == T_DIV) {
        token = parser->current_token;
        if (token->type == T_MUL) {
            parser_eat(parser, T_MUL);
        }
        else if (token->type == T_DIV) {
            parser_eat(parser, T_DIV);
        }
        node = binop_new(node, token, parser_factor(parser));
    }

    return node;
}

node_t *parser_expr(parser_t *parser) {
    token_t *token;

    node_t *node = parser_term(parser);
    while (parser->current_token->type == T_PLUS || parser->current_token->type == T_MINUS) {
        token = parser->current_token;
        if (token->type == T_PLUS) {
            parser_eat(parser, T_PLUS);
        }
        else if (token->type == T_MINUS) {
            parser_eat(parser, T_MINUS);
        }
        node = binop_new(node, token, parser_term(parser));
    }

    return node;
}

node_t *parser_parse(parser_t *parser) {
    return parser_expr(parser);
}
