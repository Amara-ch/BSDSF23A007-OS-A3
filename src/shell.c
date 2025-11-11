#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "shell.h"

// Read command using GNU Readline
char* read_cmd(const char* prompt, FILE* fp) {
    char *line = readline(prompt);
    if (line && *line) {
        add_history(line);
    }
    return line;
}

// Tokenize input into arguments
char** tokenize(char* line) {
    char** tokens = malloc(sizeof(char*) * MAXARGS);
    int i = 0;
    char* token = strtok(line, " \t\n");
    while (token != NULL && i < MAXARGS - 1) {
        tokens[i++] = strdup(token);
        token = strtok(NULL, " \t\n");
    }
    tokens[i] = NULL;
    return tokens;
}

// Free memory allocated for arguments
void free_args(char** args) {
    if (!args) return;
    for (int i = 0; args[i]; i++) {
        free(args[i]);
    }
    free(args);
}

// Built-in command handler
int handle_builtin(char** arglist) {
    if (!arglist[0]) return 0;

    if (strcmp(arglist[0], "exit") == 0) {
        printf("Exiting shell...\n");
        exit(0);
    } else if (strcmp(arglist[0], "cd") == 0) {
        if (!arglist[1]) {
            fprintf(stderr, "cd: expected argument\n");
        } else if (chdir(arglist[1]) != 0) {
            perror("cd");
        }
        return 1;
    } else if (strcmp(arglist[0], "help") == 0) {
        printf("Built-in commands:\n");
        printf("  exit    : Exit the shell\n");
        printf("  cd      : Change directory\n");
        printf("  help    : Display this help message\n");
        printf("  jobs    : List jobs (not implemented)\n");
        printf("  history : Show command history\n");
        return 1;
    } else if (strcmp(arglist[0], "jobs") == 0) {
        printf("Job control not yet implemented.\n");
        return 1;
    } else if (strcmp(arglist[0], "history") == 0) {
        HIST_ENTRY **hist_list = history_list();
        if (hist_list) {
            for (int i = 0; hist_list[i]; i++) {
                printf("%d: %s\n", i + 1, hist_list[i]->line);
            }
        }
        return 1;
    }

    return 0;
}
