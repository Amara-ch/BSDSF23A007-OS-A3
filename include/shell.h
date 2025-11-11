#ifndef SHELL_H
#define SHELL_H

#define MAXARGS 64          // Maximum number of arguments
#define HISTORY_SIZE 20     // History size for feature-3

// Function declarations
char* read_cmd(const char* prompt, FILE* fp);
char** tokenize(char* line);
void free_args(char** args);
void execute(char** arglist);
int handle_builtin(char** arglist);

#endif
