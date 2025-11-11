#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

/* ---------- Configuration ---------- */
#define HISTORY_SIZE 20

/* ---------- Function Prototypes ---------- */

/* Reads a command line from the user */
char* read_cmd(char* prompt, FILE* fp);

/* Breaks the command line into tokens */
char** tokenize(char* cmdline);

/* Executes external commands (returns 0 on success) */
int execute(char** arglist);

/* Frees memory used by argument list */
void free_args(char** arglist);

/* Built-in command handler (Feature-2) */
int handle_builtin(char **arglist);

/* ---------- History functions (Feature-3) ---------- */

/* Add a command string to history (dup internally) */
void add_history_entry(const char *cmd);

/* Print stored history entries (with numbering starting at 1) */
void print_history(void);

/* Get stored command for absolute number n (1-based). Returns NULL if invalid. */
const char* get_history_command(int n);

/* Return total commands seen so far (for numbering) */
long history_total_count(void);

#endif
