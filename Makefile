CC = gcc
FLAGS = -Wall -std=c99

all: src/main.c src/exception.c src/main.h src/exception.h
	$(CC) $(FLAGS) src/main.c src/exception.c -o bin/a.out

test: src/exception.c src/exception.h tests/test.c tests/test.h
	rm -rf tests/*.out
	$(CC) $(FLAGS) src/exception.c tests/test.c -o tests/test.out
	./tests/test.out

clean:
	rm -rf bin/*
