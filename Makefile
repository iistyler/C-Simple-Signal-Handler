CC = gcc
FLAGS = -Wall -std=c99

all: src/main.c src/exception.c src/main.h src/exception.h
	$(CC) $(FLAGS) src/main.c src/exception.c -o bin/a.out

clean:
	rm -rf bin/*
