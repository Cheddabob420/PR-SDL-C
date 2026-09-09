main: main.c
	gcc -lSDL2 -lSDL2_image main.c -o main

run: main.c
	./main
