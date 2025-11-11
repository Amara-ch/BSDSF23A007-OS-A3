#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "shell.h"

// Reads command using GNU Readline
char* read_cmd(const char* prompt, FILE* fp) {
    char *line = readline(prompt);   // Read input with line editing
    if (line && *line) {             // Non-empty line
        add_history(line);           // Add to history automatically
    }
    return line;
}

// Handle built-in commands
int handle_builtin(char **arglist) {
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
        printf("Built-in commands:\n  exit\n  cd\n  help\n  history\n  jobs\n");
        return 1;
    } else if (strcmp(arglist[0], "jobs") == 0) {
        printf("Job control not implemented.\n");
        return 1;
    }
    return 0; // not a built-in
}

// Tokenize commands (I/O redirection and pipes)
int tokenize(char *line, command_t *commands) {
    int cmd_count = 0;
    char *token;
    char *saveptr;
    char *line_copy = strdup(line);

    commands[cmd_count].argv = malloc(sizeof(char*) * MAXARGS);
    commands[cmd_count].input_file = NULL;
    commands[cmd_count].output_file = NULL;
    int arg_index = 0;

    token = strtok_r(line_copy, " \t\n", &saveptr);
    while (token) {
        if (strcmp(token, "<") == 0) {
            token = strtok_r(NULL, " \t\n", &saveptr);
            commands[cmd_count].input_file = strdup(token);
        } else if (strcmp(token, ">") == 0) {
            token = strtok_r(NULL, " \t\n", &saveptr);
            commands[cmd_count].output_file = strdup(token);
        } else if (strcmp(token, "|") == 0) {
            commands[cmd_count].argv[arg_index] = NULL;
            cmd_count++;
            commands[cmd_count].argv = malloc(sizeof(char*) * MAXARGS);
            commands[cmd_count].input_file = NULL;
            commands[cmd_count].output_file = NULL;
            arg_index = 0;
        } else {
            commands[cmd_count].argv[arg_index++] = strdup(token);
        }
        token = strtok_r(NULL, " \t\n", &saveptr);
    }

    commands[cmd_count].argv[arg_index] = NULL;
    cmd_count++;
    free(line_copy);
    return cmd_count;
}
