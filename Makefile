compile:
	gcc interpreter.c token.c lexer.c parser.c ast_tree.c main.c -ggdb -o interpreter.out

valgrind:
	valgrind --leak-check=full ./interpreter.out
