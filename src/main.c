#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"

int main() {
    char* cmdline;
    char** arglist;

    while (1) {
        cmdline = read_cmd("FCIT> ", stdin);
        if (!cmdline) break; // Ctrl+D exits

        arglist = tokenize(cmdline);
        if (!handle_builtin(arglist)) {
            execute(arglist);
        }

        free_args(arglist);
        free(cmdline);
    }

    printf("\nExiting shell.\n");
    return 0;
}

