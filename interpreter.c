#include <malloc.h>
#include <stdlib.h>
#include <ctype.h>

#ifndef NULL
#define NULL (void *)NULL
#endif

#define T_INTEGER 1
#define T_PLUS    2
#define T_EOF     3
#define T_NULL    (char)0


typedef struct token_s token_t;
struct token_s {
    int type;
    char value;
};

token_t *token_new(int type, char value) {
    token_t *token = (token_t *) calloc(1, sizeof(token_t));
    token->type = type;
    token->value = value;
    return token;
}

void token_free(token_t* token) {
    printf("token_free(%p)\n", (void *)&token);

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
    sprintf(str, "Token(%s, %c)", token_type, token->value);
    printf("%s\n", str);
    free(str);
}

typedef struct interpreter_s interpreter_t;
struct interpreter_s {
    char *text;
    unsigned int text_length;
    unsigned int pos;
    token_t *current_token;
};

interpreter_t *interpreter_new(char *text, unsigned int len) {
    interpreter_t *interpreter = (interpreter_t *) calloc(1, sizeof(interpreter_t));
    interpreter->text = text;
    interpreter->text_length = len;
    interpreter->pos = 0;
    interpreter->current_token = NULL;
    return interpreter;
}

void interpreter_free(interpreter_t *interpreter) {
    free(interpreter->current_token);
    free(interpreter);
}

void interpreter_error(interpreter_t *interpreter) {
    puts("Error parsing input\n");
    exit(1);
}

token_t *interpreter_next_token(interpreter_t *interpreter) {
    token_t *token;
    if (interpreter->pos > interpreter->text_length - 1) {
        token = token_new(T_EOF, T_NULL);
        return token;
    }

    char current_char = interpreter->text[interpreter->pos];
    if (isdigit(current_char)) {
        token = token_new(T_INTEGER, current_char);
        interpreter->pos += 1;
        return token;
    }

    if (current_char == '+') {
        token = token_new(T_PLUS, current_char);
        interpreter->pos += 1;
        return token;
    }

    interpreter_error(interpreter);
}

void interpreter_eat(interpreter_t *interpreter, int token_type) {
    if (interpreter->current_token->type == token_type) {
        interpreter->current_token = interpreter_next_token(interpreter);
    } else {
        interpreter_error(interpreter);
    }
}

int interpreter_expr(interpreter_t *interpreter) {
    interpreter->current_token = interpreter_next_token(interpreter);

    token_t *left = interpreter->current_token;
    interpreter_eat(interpreter, T_INTEGER);
    
    token_t *op = interpreter->current_token;
    interpreter_eat(interpreter, T_PLUS);
    
    token_t *right = interpreter->current_token;
    interpreter_eat(interpreter, T_INTEGER);
    
    int result = atoi(&left->value) + atoi(&right->value);
    
    free(left); free(op); free(right);
    return result;
}

int
main(void) {
    char text[4];
    fgets(text, sizeof(text), stdin);
    interpreter_t *interpreter = interpreter_new(text, 3);
    int result = interpreter_expr(interpreter);
    printf("%d\n", result);
    interpreter_free(interpreter);

    return 0;
}
