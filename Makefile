compile:
	gcc interpreter.c -ggdb -o interpreter.out

valgrind:
	valgrind --leak-check=full ./interpreter.out
