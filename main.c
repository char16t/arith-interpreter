#include <malloc.h>
#include <stdlib.h>
#include <ctype.h>

#include "token.h"
#include "lexer.h"
#include "ast_tree.h"
#include "parser.h"
#include "interpreter.h"

#ifndef NULL
#define NULL (void *)0
#endif


int
main(void) {
/*
    char text[5];
    fgets(text, sizeof(text), stdin);
    parser_t *parser = parser_new(text, 4);
    int result = parser_expr(parser);
    printf("%d\n", result);
    parser_free(parser);
*/
    printf("32 - 1 = ");
    lexer_t *lexer = lexer_new("32 - 1", 6);
    parser_t *parser = parser_new(lexer);
    interpreter_t *interpreter = interpreter_new(parser);
    int result = interpreter_interpret(interpreter);
    printf("%d\n", result);
    lexer_free(lexer);
    parser_free(parser);
    interpreter_free(interpreter);

    printf("32 + 1 = ");
    lexer = lexer_new("32 + 1", 6);
    parser = parser_new(lexer);
    interpreter = interpreter_new(parser);
    result = interpreter_interpret(interpreter);
    printf("%d\n", result);
    lexer_free(lexer);
    parser_free(parser);
    interpreter_free(interpreter);

    printf("40 + 2 = ");
    lexer = lexer_new("40 + 2", 6);
    parser = parser_new(lexer);
    interpreter = interpreter_new(parser);
    result = interpreter_interpret(interpreter);
    printf("%d\n", result);
    lexer_free(lexer);
    parser_free(parser);
    interpreter_free(interpreter);

    printf("1  +  2 + 3 = ");
    lexer = lexer_new("1  +  2 + 3", 11);
    parser = parser_new(lexer);
    interpreter = interpreter_new(parser);
    result = interpreter_interpret(interpreter);
    printf("%d\n", result);
    lexer_free(lexer);
    parser_free(parser);
    interpreter_free(interpreter);

    printf("2+ 2 * 3 = ");
    lexer = lexer_new("2+2*3", 5);
    parser = parser_new(lexer);
    interpreter = interpreter_new(parser);
    result = interpreter_interpret(interpreter);
    printf("%d\n", result);
    lexer_free(lexer);
    parser_free(parser);
    interpreter_free(interpreter);

    printf("((2+ 2)) * 3 = ");
    lexer = lexer_new("((2+2))*3", 9);
    parser = parser_new(lexer);
    interpreter = interpreter_new(parser);
    result = interpreter_interpret(interpreter);
    printf("%d\n", result);
    lexer_free(lexer);
    parser_free(parser);
    interpreter_free(interpreter);

    printf("5+-2 = ");
    lexer = lexer_new("5+-2", 4);
    parser = parser_new(lexer);
    interpreter = interpreter_new(parser);
    result = interpreter_interpret(interpreter);
    printf("%d\n", result);
    lexer_free(lexer);
    parser_free(parser);
    interpreter_free(interpreter);

    printf("5+--2 = ");
    lexer = lexer_new("5+--2", 5);
    parser = parser_new(lexer);
    interpreter = interpreter_new(parser);
    result = interpreter_interpret(interpreter);
    printf("%d\n", result);
    lexer_free(lexer);
    parser_free(parser);
    interpreter_free(interpreter);

    return 0;
}
