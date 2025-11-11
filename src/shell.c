#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "shell.h"

char *read_input() {
    char *line = readline("myshell> ");
    if (line && *line)
        add_history(line);
    return line;
}

void execute_command_from_string(char *cmd_str) {
    command_t commands[MAX_COMMANDS];
    int num_cmds = tokenize(cmd_str, commands);
    for (int i = 0; i < num_cmds; i++)
        execute_command(&commands[i], commands[i].background);
}

int execute_if_block(const char *cmd_str) {
    command_t commands[MAX_COMMANDS];
    char temp[1024];
    strncpy(temp, cmd_str, sizeof(temp));
    int num_cmds = tokenize(temp, commands);
    int status = 1;

    for (int i = 0; i < num_cmds; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            execvp(commands[i].args[0], commands[i].args);
            perror("execvp");
            exit(1);
        } else if (pid > 0) {
            waitpid(pid, &status, 0);
        } else {
            perror("fork");
            return -1;
        }
    }

    return WEXITSTATUS(status);
}

void handle_if_structure() {
    char line[1024];
    char if_cmd[1024] = {0};
    char then_block[50][1024];
    char else_block[50][1024];
    int then_count = 0, else_count = 0, in_else = 0;

    printf("(if) condition command:\n");
    if (!fgets(if_cmd, sizeof(if_cmd), stdin)) return;
    if_cmd[strcspn(if_cmd, "\n")] = 0;

    printf("(if) Enter script lines (then/else/fi to control):\n");
    while (fgets(line, sizeof(line), stdin)) {
        line[strcspn(line, "\n")] = 0;
        if (strcmp(line, "then") == 0) continue;
        if (strcmp(line, "else") == 0) { in_else = 1; continue; }
        if (strcmp(line, "fi") == 0) break;

        if (!in_else)
            strncpy(then_block[then_count++], line, sizeof(line));
        else
            strncpy(else_block[else_count++], line, sizeof(line));
    }

    int result = execute_if_block(if_cmd);
    if (result == 0) {
        for (int i = 0; i < then_count; i++)
            execute_command_from_string(then_block[i]);
    } else {
        for (int i = 0; i < else_count; i++)
            execute_command_from_string(else_block[i]);
    }
}

void process_input(char *line) {
    if (!line) return;

    if (strncmp(line, "if", 2) == 0) {
        handle_if_structure();
        free(line);
        return;
    }

    command_t commands[MAX_COMMANDS];
    int num_cmds = tokenize(line, commands);
    for (int i = 0; i < num_cmds; i++)
        execute_command(&commands[i], commands[i].background);

    free(line);
}

void shell_loop() {
    while (1) {
        char *line = read_input();
        if (!line) break;
        process_input(line);
    }
}
