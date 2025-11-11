# ---- Makefile for myshell ----
CC = gcc
CFLAGS = -Wall -g -Iinclude
OBJDIR = obj
BINDIR = bin
TARGET = $(BINDIR)/myshell
OBJS = $(OBJDIR)/main.o $(OBJDIR)/execute.o $(OBJDIR)/shell.o

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

$(OBJDIR)/%.o: src/%.c
	mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR)/*.o $(TARGET)
