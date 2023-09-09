run: src/main.c
	gcc -g -Wall -o build/compiler src/main.c src/lexer.h src/lexer.c
	./build/compiler