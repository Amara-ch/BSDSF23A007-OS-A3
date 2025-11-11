# Compiler and flags
CC = gcc
CFLAGS = -Wall -g -Iinclude

# Directories
OBJ_DIR = obj
BIN_DIR = bin
SRC_DIR = src

# Source files
SRCS = $(SRC_DIR)/main.c $(SRC_DIR)/shell.c $(SRC_DIR)/tokenize.c $(SRC_DIR)/execute.c $(SRC_DIR)/variables.c

# Object files
OBJS = $(OBJ_DIR)/main.o $(OBJ_DIR)/shell.o $(OBJ_DIR)/tokenize.o $(OBJ_DIR)/execute.o $(OBJ_DIR)/variables.o

# Target binary
TARGET = $(BIN_DIR)/myshell

# Default rule
all: $(TARGET)

# Rule to link objects into binary
$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) -lreadline

# Compile each source file into object file
$(OBJ_DIR)/main.o: $(SRC_DIR)/main.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/shell.o: $(SRC_DIR)/shell.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/tokenize.o: $(SRC_DIR)/tokenize.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/execute.o: $(SRC_DIR)/execute.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/variables.o: $(SRC_DIR)/variables.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -rf $(OBJ_DIR)/*.o $(TARGET)

.PHONY: all clean
