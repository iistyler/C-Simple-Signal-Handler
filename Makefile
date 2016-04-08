CC = gcc
FLAGS = -Wall -pedantic -std=c99

all: src/main.c src/exceptionHandler.c src/main.h src/exceptionHandler.h
	$(CC) $(FLAGS) src/main.c src/exceptionHandler.c -o bin/a.out

clean:
	rm -rf bin/*