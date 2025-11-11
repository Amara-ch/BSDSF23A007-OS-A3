#ifndef SHELL_H
#define SHELL_H

#include <stddef.h>   /* for size_t */
#include <stdio.h>

#define MAX_ARGS 100
#define MAX_COMMANDS 20
#define MAX_BG_JOBS 50

typedef struct command {
    char *args[MAX_ARGS];    /* argv-style null-terminated array */
    char *input_file;        /* filename for '<' or NULL */
    char *output_file;       /* filename for '>' or NULL */
    int pipe_to_next;        /* 1 if this cmd pipes to the next */
    int background;          /* 1 if ends with & */
} command_t;

typedef struct bg_job {
    int pid;
    char cmdline[256];  /* store command line string for jobs listing */
} bg_job_t;

/* Readline-based read function */
char* read_cmd(const char* prompt, FILE* fp);

/* Tokenize input line into commands */
int tokenize(char *line, command_t *commands);

/* Execute N commands (pipeline or single) */
void execute(command_t *commands, int ncmds);

#endif /* SHELL_H */
