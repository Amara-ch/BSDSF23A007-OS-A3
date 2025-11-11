#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>

#define MAX_ARGS 100
#define MAX_COMMANDS 100

typedef struct {
    char *args[MAX_ARGS];
    int background;
} command_t;

void shell_loop();
void process_input(char *line);
char *read_input();
int tokenize(char *line, command_t *commands);
void execute_command(command_t *cmd, int background);

// --- Feature 7 ---
void handle_if_structure();
int execute_if_block(const char *cmd);
void execute_command_from_string(char *cmd_str);

#endif
