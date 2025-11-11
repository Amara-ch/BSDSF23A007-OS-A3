#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>   // Needed for waitpid()

/* ---------- Function Prototypes ---------- */

/* Reads a command line from the user */
char* read_cmd(char* prompt, FILE* fp);

/* Breaks the command line into tokens */
char** tokenize(char* cmdline);

/* Executes external commands */
int execute(char** arglist);

/* Frees memory used by argument list */
void free_args(char** arglist);

/* Built-in command handler (Feature-2) */
int handle_builtin(char **arglist);

#endif
