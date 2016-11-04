#include <malloc.h>
#include <stdlib.h>
#include <ctype.h>

#ifndef NULL
#define NULL (void *)NULL
#endif

#define T_INTEGER 1
#define T_PLUS    2
#define T_EOF     3
#define T_MINUS   4

typedef struct token_s token_t;
struct token_s {
    int type;
    int value;
};

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

typedef struct interpreter_s interpreter_t;
struct interpreter_s {
    char *text;
    unsigned int text_length;
    unsigned int pos;
    char current_char;
    token_t *current_token;
};

interpreter_t *interpreter_new(char *text, unsigned int len) {
    interpreter_t *interpreter = (interpreter_t *) calloc(1, sizeof(interpreter_t));
    interpreter->text = text;
    interpreter->text_length = len;
    interpreter->pos = 0;
    interpreter->current_char = interpreter->text[interpreter->pos];
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

void interpreter_advance(interpreter_t *interpreter) {
    interpreter->pos += 1;
    if (interpreter->pos > interpreter->text_length - 1) {
        interpreter->current_char = NULL;
    } else {
        interpreter->current_char = interpreter->text[interpreter->pos];
    }
}

void interpreter_skip_whitespaces(interpreter_t *interpreter) {
    while (interpreter->current_char && interpreter->current_char == ' ') {
        interpreter_advance(interpreter);
    }
}

int interpreter_integer(interpreter_t *interpreter) {
    char result[10];
    unsigned int i = 0;
    while (interpreter->current_char && isdigit(interpreter->current_char)) {
        result[i] = interpreter->current_char;
        interpreter_advance(interpreter);
        i++;
    }
    return atoi(result);
}

token_t *interpreter_next_token(interpreter_t *interpreter) {
    token_t *token;

    while (interpreter->current_char) {
        if (interpreter->current_char == ' ') {
            interpreter_skip_whitespaces(interpreter);
            continue;
        }

        if (isdigit(interpreter->current_char)) {
            token = token_new(T_INTEGER, interpreter_integer(interpreter));
            return token;
        }

        if (interpreter->current_char == '+') {
            interpreter_advance(interpreter);
            token = token_new(T_PLUS, '+');
            return token;
        }

        if (interpreter->current_char == '-') {
            interpreter_advance(interpreter);
            token = token_new(T_MINUS, '-');
            return token;
        }

        interpreter_error(interpreter);
    }

    token = token_new(T_EOF, (char)0);
    return token;
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
    if (op->type == T_PLUS) {
        interpreter_eat(interpreter, T_PLUS);
    } else {
        interpreter_eat(interpreter, T_MINUS);
    }

    token_t *right = interpreter->current_token;
    interpreter_eat(interpreter, T_INTEGER);
    
    int result;
    
    if (op->type == T_PLUS)
        result = left->value + right->value;
    else
        result = left->value - right->value;

    free(left); free(op); free(right);
    return result;
}

int
main(void) {
/*
    char text[5];
    fgets(text, sizeof(text), stdin);
    interpreter_t *interpreter = interpreter_new(text, 4);
    int result = interpreter_expr(interpreter);
    printf("%d\n", result);
    interpreter_free(interpreter);
*/
    interpreter_t *interpreter = interpreter_new("32 - 1", 6);
    int result = interpreter_expr(interpreter);
    printf("%d\n", result);
    interpreter_free(interpreter);

    return 0;
}
