CC = gcc
<<<<<<< HEAD
CFLAGS = -Wall -g
=======
CFLAGS = -I./include -Wall
>>>>>>> 0c8d997ced2ecbd3860dd862a881d748394ee296
SRC = src/main.c src/shell.c src/execute.c
OBJ = obj/main.o obj/shell.o obj/execute.o
BIN = bin/myshell

all: $(BIN)

$(BIN): $(OBJ)
	mkdir -p bin
<<<<<<< HEAD
	$(CC) $(CFLAGS) -o $(BIN) $(OBJ)
=======
	$(CC) $(OBJ) -o $(BIN)
>>>>>>> 0c8d997ced2ecbd3860dd862a881d748394ee296

obj/%.o: src/%.c
	mkdir -p obj
	$(CC) $(CFLAGS) -c $< -o $@

clean:
<<<<<<< HEAD
	rm -rf obj/* bin/*
=======
	rm -rf obj/*.o $(BIN)
>>>>>>> 0c8d997ced2ecbd3860dd862a881d748394ee296
