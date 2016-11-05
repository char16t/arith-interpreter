#ifndef LEXER_H
#define LEXER_H

#include "token.h"

typedef struct lexer_s {
    char *text;
    unsigned int text_length;
    unsigned int pos;
    char current_char;
} lexer_t;

lexer_t *lexer_new(char *text, unsigned int len);

void lexer_free(lexer_t *lexer);

void lexer_error(lexer_t *lexer);

void lexer_advance(lexer_t *lexer);

void lexer_skip_whitespaces(lexer_t *lexer);

int lexer_integer(lexer_t *lexer);

token_t *lexer_next_token(lexer_t *lexer);

#endif
