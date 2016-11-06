#include <malloc.h>
#include "token.h"

token_t *token_new(int type, int value) {
    token_t *token = (token_t *) calloc(1, sizeof(token_t));
    token->type = type;
    token->value = value;
    return token;
}

void token_free(token_t* token) {
    free(token);
}

