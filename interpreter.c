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
#define T_DIV     5
#define T_MUL     6

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

typedef struct lexer_s lexer_t;
struct lexer_s {
    char *text;
    unsigned int text_length;
    unsigned int pos;
    char current_char;
};

lexer_t *lexer_new(char *text, unsigned int len) {
    lexer_t *lexer = (lexer_t *) calloc(1, sizeof(lexer));
    lexer->text = text;
    lexer->text_length = len;
    lexer->pos = 0;
    lexer->current_char = lexer->text[lexer->pos];
}

void lexer_free(lexer_t *lexer) {
    free(lexer);
}

void lexer_error(lexer_t *lexer) {
    puts("Invalid character\n");
    exit(1);
}

void lexer_advance(lexer_t *lexer) {
    lexer->pos += 1;
    if (lexer->pos > lexer->text_length - 1) {
        lexer->current_char = NULL;
    } else {
        lexer->current_char = lexer->text[lexer->pos];
    }
}

void lexer_skip_whitespaces(lexer_t *lexer) {
    while (lexer->current_char && lexer->current_char == ' ') {
        lexer_advance(lexer);
    }
}

int lexer_integer(lexer_t *lexer) {
    char result[10];
    unsigned int i = 0;
    while (lexer->current_char && isdigit(lexer->current_char)) {
        result[i] = lexer->current_char;
        lexer_advance(lexer);
        i++;
    }
    return atoi(result);
}

token_t *lexer_next_token(lexer_t *lexer) {
    token_t *token;

    while (lexer->current_char) {
        if (lexer->current_char == ' ') {
            lexer_skip_whitespaces(lexer);
            continue;
        }

        if (isdigit(lexer->current_char)) {
            token = token_new(T_INTEGER, lexer_integer(lexer));
            return token;
        }

        if (lexer->current_char == '+') {
            lexer_advance(lexer);
            token = token_new(T_PLUS, '+');
            return token;
        }

        if (lexer->current_char == '-') {
            lexer_advance(lexer);
            token = token_new(T_MINUS, '-');
            return token;
        }

        if (lexer->current_char == '*') {
            lexer_advance(lexer);
            return token_new(T_MUL, '*');
        }

        if (lexer->current_char == '/') {
            lexer_advance(lexer);
            return token_new(T_DIV, '/');
        }

        lexer_error(lexer);
    }

    token = token_new(T_EOF, (char)0);
    return token;
}

typedef struct interpreter_s interpreter_t;
struct interpreter_s {
    lexer_t *lexer;
    token_t *current_token;
};

interpreter_t *interpreter_new(lexer_t *lexer) {
    interpreter_t *interpreter = (interpreter_t *) calloc(1, sizeof(interpreter_t));
    interpreter->lexer = lexer;
    interpreter->current_token = NULL;
    return interpreter;
}

void interpreter_free(interpreter_t *interpreter) {
    free(interpreter->current_token);
    free(interpreter);
}

void interpreter_error(interpreter_t *interpreter) {
    puts("Invalid token\n");
    exit(1);
}

void interpreter_eat(interpreter_t *interpreter, int token_type) {
    if (interpreter->current_token->type == token_type) {
        interpreter->current_token = lexer_next_token(interpreter->lexer);
    } else {
        interpreter_error(interpreter);
    }
}

int interpreter_factor(interpreter_t *interpreter) {
    token_t *token = interpreter->current_token;
    interpreter_eat(interpreter, T_INTEGER);

    int value = token->value;
    free(token);

    return value;
}

int interpreter_term(interpreter_t *interpreter) {
    token_t *token;

    int result = interpreter_factor(interpreter);

    while (interpreter->current_token->type == T_MUL || interpreter->current_token->type == T_DIV) {
        token = interpreter->current_token;
        if (token->type == T_MUL) {
            interpreter_eat(interpreter, T_MUL);
            result *= interpreter_term(interpreter);
        }
        else if (token->type == T_DIV) {
            interpreter_eat(interpreter, T_DIV);
            result /= interpreter_term(interpreter);
        }
        free(token);
    }

    return result;
}

int interpreter_expr(interpreter_t *interpreter) {
    token_t *token;

    interpreter->current_token = lexer_next_token(interpreter->lexer);
    int result = interpreter_term(interpreter);
    while (interpreter->current_token->type == T_PLUS || interpreter->current_token->type == T_MINUS) {
        token = interpreter->current_token;
        if (token->type == T_PLUS) {
            interpreter_eat(interpreter, T_PLUS);
            result += interpreter_term(interpreter);
        }
        else if (token->type == T_MINUS) {
            interpreter_eat(interpreter, T_MINUS);
            result -= interpreter_term(interpreter);
        }
        free(token);
    }

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
    printf("32 - 1 = ");
    lexer_t *lexer = lexer_new("32 - 1", 6);
    interpreter_t *interpreter = interpreter_new(lexer);
    int result = interpreter_expr(interpreter);
    printf("%d\n", result);
    lexer_free(lexer);
    interpreter_free(interpreter);

    printf("32 + 1 = ");
    lexer = lexer_new("32 + 1", 6);
    interpreter = interpreter_new(lexer);
    result = interpreter_expr(interpreter);
    printf("%d\n", result);
    lexer_free(lexer);
    interpreter_free(interpreter);

    printf("40 + 2 = ");
    lexer = lexer_new("40 + 2", 6);
    interpreter = interpreter_new(lexer);
    result = interpreter_expr(interpreter);
    printf("%d\n", result);
    lexer_free(lexer);
    interpreter_free(interpreter);

    printf("1  +  2 + 3 = ");
    lexer = lexer_new("1  +  2 + 3", 11);
    interpreter = interpreter_new(lexer);
    result = interpreter_expr(interpreter);
    printf("%d\n", result);
    lexer_free(lexer);
    interpreter_free(interpreter);

    printf("2+ 2 * 3 = ");
    lexer = lexer_new("2+2*3", 5);
    interpreter = interpreter_new(lexer);
    result = interpreter_expr(interpreter);
    printf("%d\n", result);
    lexer_free(lexer);
    interpreter_free(interpreter);

    return 0;
}
