#include <malloc.h>
#include <stdlib.h>
#include <ctype.h>
#include "lexer.h"

lexer_t *lexer_new(char *text, unsigned int len) {
    lexer_t *lexer = (lexer_t *) calloc(1, sizeof(lexer));
    lexer->text = text;
    lexer->text_length = len;
    lexer->pos = 0;
    lexer->current_char = lexer->text[lexer->pos];
    return lexer;
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
            token = token_new_int(T_INTEGER, lexer_integer(lexer));
            return token;
        }

        if (lexer->current_char == '+') {
            lexer_advance(lexer);
            token = token_new_str(T_PLUS, "+", 1);
            free(token->value.c);
            return token;
        }

        if (lexer->current_char == '-') {
            lexer_advance(lexer);
            token = token_new_str(T_MINUS, "-", 1);
            return token;
        }

        if (lexer->current_char == '*') {
            lexer_advance(lexer);
            token = token_new_str(T_MUL, "*", 1);
            free(token->value.c);
            return token;
        }

        if (lexer->current_char == '/') {
            lexer_advance(lexer);
            token = token_new_str(T_DIV, "/", 1);
            free(token->value.c);
            return token;
        }

        if (lexer->current_char == '(') {
            lexer_advance(lexer);
            token = token_new_str(T_LPAREN, "(", 1);
            free(token->value.c);
            return token;
        }

        if (lexer->current_char == ')') {
            lexer_advance(lexer);
            token = token_new_str(T_RPAREN, ")", 1);
            free(token->value.c);
            return token;
        }

        lexer_error(lexer);
    }

    token = token_new_str(T_EOF, "", 1);
    return token;
}
