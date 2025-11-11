#include <string.h>
#include <stdlib.h>
#include "shell.h"

int tokenize(char *line, command_t *commands) {
    char *cmd_str = strtok(line, ";");
    int cmd_count = 0;

    while (cmd_str != NULL && cmd_count < MAX_COMMANDS) {
        command_t *cmd = &commands[cmd_count];
        cmd->background = 0;

        // Trim spaces
        while (*cmd_str == ' ') cmd_str++;

        // Check for &
        size_t len = strlen(cmd_str);
        while (len > 0 && cmd_str[len - 1] == ' ') cmd_str[--len] = '\0';
        if (len > 0 && cmd_str[len - 1] == '&') {
            cmd->background = 1;
            cmd_str[len - 1] = '\0';
        }

        int argc = 0;
        char *token = strtok(cmd_str, " \t");
        while (token && argc < MAX_ARGS - 1) {
            cmd->args[argc++] = strdup(token);
            token = strtok(NULL, " \t");
        }
        cmd->args[argc] = NULL;

        cmd_count++;
        cmd_str = strtok(NULL, ";");
    }

    return cmd_count;
}
