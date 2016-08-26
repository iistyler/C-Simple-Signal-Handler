CC = gcc
FLAGS = -Wall -std=c99
TFLAGS = -w -std=c99
RELEASE_FLAGS = -Wall -std=c99

all: src/main.c src/exception.c src/main.h src/exception.h
	mkdir -p bin
	$(CC) $(FLAGS) src/main.c src/exception.c -o bin/a.out

test: src/exception.c src/exception.h tests/test.c tests/test.h
	rm -rf tests/*.out
	$(CC) $(FLAGS) src/exception.c tests/test.c -o tests/test.out
	./tests/test.out

release: src/exception.c src/exception.h tests/test.c tests/test.h tests/errHeaders.h
	mkdir -p release
	rm -rf release/*
	$(CC) $(TFLAGS) src/exception.c tests/test.c -o tests/test.out
	./tests/test.out -hideDetails
	$(CC) $(RELEASE_FLAGS) -c src/exception.c $(shell cat tests/err) -o release/exception.o

release-force: src/exception.c src/exception.h
	mkdir -p release
	rm -rf release/*
	$(CC) $(RELEASE_FLAGS) -c src/exception.c -o release/exception.o

clean:
	rm -rf bin/*
	rm -rf release/*
	rm -rf tests/*.out
	rm -rf tests/*.o
