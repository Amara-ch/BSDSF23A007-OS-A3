#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>

// Maximum arguments per command
#define MAXARGS 64

// Command structure for I/O redirection and pipes
typedef struct command {
    char **argv;           // Argument list
    char *input_file;      // Input redirection file
    char *output_file;     // Output redirection file
} command_t;

// Function declarations
char* read_cmd(const char* prompt, FILE* fp);
int handle_builtin(char **arglist);

int tokenize(char *line, command_t *commands);       // returns number of commands
void execute(command_t *commands, int ncmds);

#endif
