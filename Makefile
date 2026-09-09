main: main.c
	gcc -lSDL2 main.c -o main

run: main.c
	./main
