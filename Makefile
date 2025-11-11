CC = gcc
CFLAGS = -Wall -g -Iinclude
LDFLAGS = -lreadline

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

SRCS = $(SRC_DIR)/main.c $(SRC_DIR)/execute.c $(SRC_DIR)/shell.c
OBJS = $(OBJ_DIR)/main.o $(OBJ_DIR)/execute.o $(OBJ_DIR)/shell.o
TARGET = $(BIN_DIR)/myshell

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)/*.o $(BIN_DIR)/myshell
