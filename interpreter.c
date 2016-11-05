#include <malloc.h>
#include <stdlib.h>
#include <ctype.h>

#ifndef NULL
#define NULL (void *)0
#endif

#define T_INTEGER 1
#define T_PLUS    2
#define T_EOF     3
#define T_MINUS   4
#define T_DIV     5
#define T_MUL     6
#define T_LPAREN  7
#define T_RPAREN  8

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

        if (lexer->current_char == '(') {
            lexer_advance(lexer);
            return token_new(T_LPAREN, '(');
        }

        if (lexer->current_char == ')') {
            lexer_advance(lexer);
            return token_new(T_RPAREN, ')');
        }

        lexer_error(lexer);
    }

    token = token_new(T_EOF, (char)0);
    return token;
}


#define TYPE_BINOP   0
#define TYPE_NUM     1
#define TYPE_UNARYOP 2
typedef struct node_s node_t;
struct node_s {
    char type; /* 0 - binop; 1 - num; 2 - unaryop */
    node_t  *left;
    node_t  *right;
    token_t *token;
    token_t *op;
    int value;
};

node_t *binop_new(node_t *left, token_t *op, node_t *right) {
    node_t *node = (node_t *) calloc(1, sizeof(node_t));
    node->type = TYPE_BINOP;
    node->left = left;
    node->right = right;
    node->op = op;
    node->token = op;
    return node;
}

void binop_free(node_t *binop) {
    free(binop->token);
    free(binop);
}

node_t *num_new(token_t *token) {
    node_t *node = (node_t *) calloc(1, sizeof(node_t));
    node->type = TYPE_NUM;
    node->token = token;
    node->value = token->value;
    return node;
}

void num_free(node_t *num) {
    free(num->token);
    free(num);
}

node_t *unaryop_new(token_t *op, node_t *expr) {
    node_t *node = (node_t *) calloc(1, sizeof(node_t));
    node->type = TYPE_UNARYOP;
    node->token = op;
    node->op = op;
    node->right = expr;
    return node;
}

void unaryop_free(node_t *unaryop) {
    free(unaryop->token);
    free(unaryop);
}

typedef struct parser_s parser_t;
struct parser_s {
    lexer_t *lexer;
    token_t *current_token;
};

parser_t *parser_new(lexer_t *lexer) {
    parser_t *parser = (parser_t *) calloc(1, sizeof(parser_t));
    parser->lexer = lexer;
    parser->current_token = lexer_next_token(parser->lexer);
    return parser;
}

void parser_free(parser_t *parser) {
    free(parser->current_token);
    free(parser);
}

void parser_error(parser_t *parser) {
    puts("Invalid token\n");
    exit(1);
}

token_t *parser_eat(parser_t *parser, int token_type) {
    if (parser->current_token->type == token_type) {
        token_t *prev = parser->current_token;
        parser->current_token = lexer_next_token(parser->lexer);
        return prev;
    } else {
        parser_error(parser);
    }
}

node_t *parser_expr(parser_t *parser);

node_t *parser_factor(parser_t *parser) {
    token_t *token = parser->current_token;

    if (token->type == T_PLUS) {
        parser_eat(parser, T_PLUS);
        return unaryop_new(token, parser_factor(parser));
    }

    else if (token->type == T_MINUS) {
        parser_eat(parser, T_MINUS);
        return unaryop_new(token, parser_factor(parser));
    }

    else if (token->type == T_INTEGER) {
        parser_eat(parser, T_INTEGER);
        return num_new(token);
        //free(token);
    }
    else if (token->type == T_LPAREN) {
        token_t *lp = parser_eat(parser, T_LPAREN);
        node_t *node = parser_expr(parser);
        token_t *rp = parser_eat(parser, T_RPAREN);
        free(lp); free(rp);
        return node;
    }
}

node_t *parser_term(parser_t *parser) {
    token_t *token;
    token_t *for_free;

    node_t *node = parser_factor(parser);
    while (parser->current_token->type == T_MUL || parser->current_token->type == T_DIV) {
        token = parser->current_token;
        if (token->type == T_MUL) {
            parser_eat(parser, T_MUL);
        }
        else if (token->type == T_DIV) {
            parser_eat(parser, T_DIV);
        }
        node = binop_new(node, token, parser_factor(parser));
        //free(token);
    }

    return node;
}

node_t *parser_expr(parser_t *parser) {
    token_t *token;

    node_t *node = parser_term(parser);
    while (parser->current_token->type == T_PLUS || parser->current_token->type == T_MINUS) {
        token = parser->current_token;
        if (token->type == T_PLUS) {
            parser_eat(parser, T_PLUS);
        }
        else if (token->type == T_MINUS) {
            parser_eat(parser, T_MINUS);
        }
        node = binop_new(node, token, parser_term(parser));

        //free(token);
    }

    return node;
}

node_t *parser_parse(parser_t *parser) {
    return parser_expr(parser);
}
typedef struct interpreter_s interpreter_t;
struct interpreter_s {
    parser_t *parser;
};

interpreter_t *interpreter_new(parser_t *parser) {
    interpreter_t *interpreter = (interpreter_t *) calloc(1, sizeof(interpreter_t));
    interpreter->parser = parser;
    return interpreter;
}

void interpreter_free(interpreter_t *interpreter) {
    free(interpreter);
}

int interpreter_visit_num(interpreter_t *interpreter, node_t *node);
int interpreter_visit_binop(interpreter_t *interpreter, node_t *node);
int interpreter_visit_unaryop(interpreter_t *interpreter, node_t *node);

int interpreter_visit(interpreter_t *interpreter, node_t *node) {
    int value;

    if (node->type == TYPE_BINOP) {
        value = interpreter_visit_binop(interpreter, node);
        binop_free(node);
    }
    
    else if (node->type == TYPE_NUM) {
        value = interpreter_visit_num(interpreter, node);
        num_free(node);
    }

    else if (node->type == TYPE_UNARYOP) {
        value = interpreter_visit_unaryop(interpreter, node);
        unaryop_free(node);
    }

    return value;
}

int interpreter_visit_num(interpreter_t *interpreter, node_t *node) {
    return node->value;
}

int interpreter_visit_unaryop(interpreter_t *interpreter, node_t *node) {
    if (node->op->type == T_PLUS) {
        return interpreter_visit(interpreter, node->right);
    }

    if (node->op->type == T_MINUS) {
        return (-1)*interpreter_visit(interpreter, node->right); 
    }
}

int interpreter_visit_binop(interpreter_t *interpreter, node_t *node) {
    if (node->op->type == T_PLUS) {
        return interpreter_visit(interpreter, node->left) + interpreter_visit(interpreter, node->right);
    }
    else if (node->op->type == T_MINUS) {
        return interpreter_visit(interpreter, node->left) - interpreter_visit(interpreter, node->right);
    }
    else if (node->op->type == T_MUL) {
        return interpreter_visit(interpreter, node->left) * interpreter_visit(interpreter, node->right);
    }
    else if (node->op->type == T_DIV) {
        return interpreter_visit(interpreter, node->left) / interpreter_visit(interpreter, node->right);
    }
}

int interpreter_interpret(interpreter_t *interpreter) {
    node_t *tree = parser_parse(interpreter->parser);
    return interpreter_visit(interpreter, tree);
}

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
