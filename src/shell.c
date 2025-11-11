#include "shell.h"

/* -------------------- read_cmd -------------------- */
char* read_cmd(char* prompt, FILE* fp) {
    char* cmdline = NULL;
    size_t size = 0;

    if (prompt != NULL) {
        printf("%s", prompt);
        fflush(stdout);
    }
    if (getline(&cmdline, &size, fp) == -1) {
        if (feof(fp)) {
            /* EOF from stdin (Ctrl+D) */
            free(cmdline);
            return NULL;
        } else {
            perror("getline");
            free(cmdline);
            return NULL;
        }
    }
    return cmdline;
}

/* -------------------- tokenize -------------------- */
char** tokenize(char* cmdline) {
    if (cmdline == NULL) return NULL;

    int bufsize = 64;
    char** tokens = malloc(bufsize * sizeof(char*));
    if (!tokens) {
        fprintf(stderr, "Allocation error\n");
        return NULL;
    }

    int position = 0;
    char *token = strtok(cmdline, " \t\r\n");
    while (token != NULL) {
        tokens[position++] = token;
        if (position >= bufsize) {
            bufsize += 64;
            tokens = realloc(tokens, bufsize * sizeof(char*));
            if (!tokens) {
                fprintf(stderr, "Allocation error\n");
                return NULL;
            }
        }
        token = strtok(NULL, " \t\r\n");
    }
    tokens[position] = NULL;
    return tokens;
}

/* -------------------- free_args -------------------- */
void free_args(char** arglist) {
    if (arglist) free(arglist);
}

/* ---------------- History storage (circular buffer) ---------------- */

static char *history_buf[HISTORY_SIZE]; /* pointers to strdup'ed strings */
static int history_count = 0;           /* current number of stored items (<= HISTORY_SIZE) */
static int history_start = 0;           /* index of oldest entry in buffer */
static long history_total = 0;          /* total commands ever added (for numbering) */

void add_history_entry(const char *cmd) {
    if (cmd == NULL) return;

    /* Duplicate the string to own it */
    char *copy = strdup(cmd);
    if (!copy) return;

    /* If buffer is not full, place at (history_start + history_count) % HISTORY_SIZE */
    if (history_count < HISTORY_SIZE) {
        int idx = (history_start + history_count) % HISTORY_SIZE;
        history_buf[idx] = copy;
        history_count++;
    } else {
        /* Buffer full: overwrite oldest and advance start */
        free(history_buf[history_start]);
        history_buf[history_start] = copy;
        history_start = (history_start + 1) % HISTORY_SIZE;
    }
    history_total++;
}

void print_history(void) {
    long base_num = (history_total >= history_count) ? (history_total - history_count + 1) : 1;
    for (int i = 0; i < history_count; ++i) {
        int idx = (history_start + i) % HISTORY_SIZE;
        printf("%ld  %s\n", base_num + i, history_buf[idx]);
    }
}

const char* get_history_command(int n) {
    if (n <= 0) return NULL;
    long first_num = (history_total >= history_count) ? (history_total - history_count + 1) : 1;
    long last_num = first_num + history_count - 1;
    if (n < first_num || n > last_num) return NULL;
    int offset = (int)(n - first_num); /* 0-based offset into stored items */
    int idx = (history_start + offset) % HISTORY_SIZE;
    return history_buf[idx];
}

long history_total_count(void) {
    return history_total;
}

/* -------------------- Built-in handler (uses history functions) -------------------- */
int handle_builtin(char **arglist) {
    if (arglist == NULL || arglist[0] == NULL) return 0;

    if (strcmp(arglist[0], "exit") == 0) {
        /* exit gracefully */
        printf("Exiting shell...\n");
        exit(0);
    } else if (strcmp(arglist[0], "cd") == 0) {
        if (arglist[1] == NULL) {
            fprintf(stderr, "cd: expected argument\n");
        } else {
            if (chdir(arglist[1]) != 0) {
                perror("cd");
            }
        }
        return 1;
    } else if (strcmp(arglist[0], "help") == 0) {
        printf("Built-in commands:\n");
        printf("  exit  : Exit the shell\n");
        printf("  cd    : Change directory\n");
        printf("  help  : Display this help message\n");
        printf("  jobs  : Job control not yet implemented\n");
        printf("  history : Show command history (last %d)\n", HISTORY_SIZE);
        return 1;
    } else if (strcmp(arglist[0], "jobs") == 0) {
        printf("Job control not yet implemented.\n");
        return 1;
    } else if (strcmp(arglist[0], "history") == 0) {
        print_history();
        return 1;
    }

    return 0; /* not a built-in */
}
