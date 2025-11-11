#include "shell.h"

int main() {
    while (1) {
        printf("myshell> ");
        char *cmdline = read_cmd(NULL, stdin);  // Read input

        if (cmdline == NULL) {
            printf("\n");
            break; // EOF (Ctrl+D)
        }

        char **arglist = tokenize(cmdline);  // Break into tokens
        if (arglist == NULL) {
            free(cmdline);
            continue;
        }

        // Handle built-in commands first
        if (!handle_builtin(arglist)) {
            execute(arglist);  // Only run external commands if not built-in
        }

        free_args(arglist);
        free(cmdline);
    }

    return 0;
}
