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

void token_print(token_t *token) {
    
    unsigned int str_length = 10; /* 9 + '\0' */
    char *str;
    char const *token_type;

    switch (token->type) {
        case 1:
            token_type = "T_INTEGER";
            str_length += 9;
            break;
        case 2:
            token_type = "T_PLUS";
            str_length += 6;
            break;
        case 3:
            token_type = "T_EOF";
            str_length += 5;
            break;
        default:
            break;
    }

    str = (char *) calloc(str_length, sizeof(char));
    sprintf(str, "Token(%s, %d)", token_type, token->value);
    printf("%s\n", str);
    free(str);
}

