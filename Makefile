main: main.c
	gcc -lSDL2 -lSDL2_image -lSDL2_ttf main.c -o main

run: main.c
	./main
