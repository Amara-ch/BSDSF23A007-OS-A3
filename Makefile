CC = gcc
CFLAGS = -Wall -g -Iinclude
LDFLAGS = -lreadline

SRCS = src/main.c src/shell.c src/tokenize.c src/execute.c
OBJS = $(patsubst src/%.c,obj/%.o,$(SRCS))
BIN = bin/myshell

.PHONY: all clean

all: $(BIN)

$(BIN): $(OBJS)
	mkdir -p bin
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)

obj/%.o: src/%.c
	mkdir -p obj
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf obj/*.o bin/myshell
