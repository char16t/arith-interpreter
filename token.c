#include <malloc.h>
#include <string.h>
#include "token.h"

token_t *token_new(int type, token_value_t value) {
    token_t *token = (token_t *) calloc(1, sizeof(token_t));
    token->type  = type;
    token->value = value;
    return token;
}

token_t *token_new_str(int type, char *str, int length) {
    token_t *token = (token_t *) calloc(1, sizeof(token_t));
    token->type   = type;
    token->value.c = (char *) calloc(length+1, sizeof(char));
    strcpy(token->value.c, str);
    return token;
}

token_t *token_new_int(int type, int value) {
    token_t *token = (token_t *) calloc(1, sizeof(token_t));
    token->type = type;
    token->value.i = value;
    return token;
}

void token_free(token_t* token) {
    free(token);
}

