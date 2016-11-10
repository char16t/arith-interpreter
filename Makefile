compile:
	gcc list.c hashtable.c interpreter.c token.c lexer.c parser.c ast_tree.c main.c -Wall -Wextra -Wno-unused-parameter -Wno-return-type  -ggdb -o interpreter.out

valgrind:
	valgrind --leak-check=full ./interpreter.out
