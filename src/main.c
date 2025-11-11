#include "shell.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    char *cmdline;
    command_t commands[16]; // maximum 16 commands in a pipeline
    int ncmds;

    while (1) {
        cmdline = read_cmd("FCIT> ", stdin);
        if (!cmdline) break;

        ncmds = tokenize(cmdline, commands);
        if (ncmds > 0) {
            // Handle built-in only for single commands without pipes
            if (ncmds == 1 && handle_builtin(commands[0].argv) == 0) {
                execute(commands, ncmds);
            } else if (ncmds > 1) {
                execute(commands, ncmds);
            }
        }
        free(cmdline);
        for (int i = 0; i < ncmds; i++) {
            for (int j = 0; commands[i].argv[j]; j++)
                free(commands[i].argv[j]);
            free(commands[i].argv);
            if (commands[i].input_file) free(commands[i].input_file);
            if (commands[i].output_file) free(commands[i].output_file);
        }
    }
    return 0;
}
