#ifndef TOKEN_H
#define TOKEN_H

#define T_INTEGER 1
#define T_PLUS    2
#define T_EOF     3
#define T_MINUS   4
#define T_DIV     5
#define T_MUL     6
#define T_LPAREN  7
#define T_RPAREN  8

typedef struct token_s {
    int type;
    int value;
} token_t;

token_t *token_new(int type, int value);

void token_free(token_t* token);

#endif
