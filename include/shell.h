#ifndef SHELL_H
#define SHELL_H

#include <stddef.h>     // for size_t
#include <sys/types.h>  // for pid_t
#include <stdio.h>

#define MAX_COMMANDS 100
#define MAX_BG_JOBS 50
#define MAX_CMDLINE 1024

typedef struct {
    pid_t pid;
    char cmdline[MAX_CMDLINE];
} bg_job_t;

// Background job list
extern bg_job_t bg_jobs[MAX_BG_JOBS];
extern int bg_count;

typedef struct {
    char *args[100]; // arguments
    int argc;
} command_t;

// Shell functions
char* read_cmd(const char* prompt);
void shell_loop();
void process_input(char *line);
void check_bg_jobs();
void add_bg_job(pid_t pid, const char *cmdline);
void show_jobs();

#endif
