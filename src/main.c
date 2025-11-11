#include "shell.h"
#include <ctype.h>

/* Helper: trim leading spaces */
static char* ltrim(char* s) {
    while(*s && isspace((unsigned char)*s)) s++;
    return s;
}

int main() {
    char *raw = NULL;

    while (1) {
        /* Read a command line */
        raw = read_cmd("myshell> ", stdin);
        if (raw == NULL) {
            /* EOF or read error -> exit shell */
            printf("\n");
            break;
        }

        /* Trim leading spaces */
        char *line = ltrim(raw);

        /* Skip empty lines */
        if (line[0] == '\0') {
            free(raw);
            continue;
        }

        /* Special: handle !n before tokenization and before adding to history */
        if (line[0] == '!' && isdigit((unsigned char)line[1])) {
            int n = atoi(line + 1);
            const char *cmd = get_history_command(n);
            if (cmd == NULL) {
                printf("No such command in history: %d\n", n);
                free(raw);
                continue;
            }
            /* Replace line with a duplicate of the history command */
            free(raw);
            raw = strdup(cmd);
            if (!raw) {
                fprintf(stderr, "Memory error\n");
                continue;
            }
            line = raw;
            /* Do NOT add the '!n' string to history. We'll add the actual command below. */
        }

        /* Now line holds the command string to execute (either original or expanded) */
        /* Add to history (store a copy) */
        add_history_entry(line);

        /* Tokenize: note tokenize modifies the buffer, so we pass a mutable buffer */
        char *working = strdup(line);  /* duplicate so history storage remains intact */
        if (!working) {
            fprintf(stderr, "Allocation failed\n");
            free(raw);
            continue;
        }

        char **arglist = tokenize(working);
        if (arglist == NULL) {
            free(working);
            free(raw);
            continue;
        }

        /* Handle built-ins first; if not a built-in, execute as external */
        if (!handle_builtin(arglist)) {
            execute(arglist);
        }

        /* Cleanup: free token buffer and arglist and raw line */
        free_args(arglist);
        free(working);
        free(raw);
    }

    return 0;
}
