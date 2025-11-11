#include "shell.h"

/* ---------------------------------------------------------
   Function: read_cmd
   Reads a command line from the user.
--------------------------------------------------------- */
char* read_cmd(char* prompt, FILE* fp) {
    char* cmdline = NULL;
    size_t size = 0;

    printf("%s", prompt);
    if (getline(&cmdline, &size, fp) == -1) {
        if (feof(fp)) {
            printf("\n");
            exit(0);
        } else {
            perror("getline");
            exit(1);
        }
    }
    return cmdline;
}

/* ---------------------------------------------------------
   Function: tokenize
   Splits command line into tokens.
--------------------------------------------------------- */
char** tokenize(char* cmdline) {
    char** arglist = malloc(64 * sizeof(char*));
    char* token;
    int position = 0;

    if (!arglist) {
        fprintf(stderr, "Allocation error\n");
        exit(1);
    }

    token = strtok(cmdline, " \t\r\n");
    while (token != NULL) {
        arglist[position++] = token;
        token = strtok(NULL, " \t\r\n");
    }
    arglist[position] = NULL;
    return arglist;
}

/* ---------------------------------------------------------
   Function: free_args
   Frees memory used by arglist.
--------------------------------------------------------- */
void free_args(char** arglist) {
    free(arglist);
}

/* ---------------------------------------------------------
   Function: handle_builtin
   Handles built-in shell commands.
--------------------------------------------------------- */
int handle_builtin(char **arglist) {
    if (arglist[0] == NULL)
        return 1;  // empty command handled

    if (strcmp(arglist[0], "exit") == 0) {
        printf("Exiting shell...\n");
        exit(0);
    } 
    else if (strcmp(arglist[0], "cd") == 0) {
        if (arglist[1] == NULL) {
            fprintf(stderr, "cd: expected argument\n");
        } else {
            if (chdir(arglist[1]) != 0) {
                perror("cd");
            }
        }
        return 1;
    } 
    else if (strcmp(arglist[0], "help") == 0) {
        printf("Built-in commands:\n");
        printf("  exit  : Exit the shell\n");
        printf("  cd    : Change directory\n");
        printf("  help  : Display this help message\n");
        printf("  jobs  : List jobs (not implemented)\n");
        return 1;
    } 
    else if (strcmp(arglist[0], "jobs") == 0) {
        printf("Job control not yet implemented.\n");
        return 1;
    }

    return 0;  // not a built-in
}
