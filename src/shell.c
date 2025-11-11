#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <stddef.h>
#include "shell.h"
#include "tokenize.h"
#include "execute.h"
#include "variables.h"

#define MAX_COMMANDS 20
#define MAX_BG_JOBS 50

// Background job struct
typedef struct {
    pid_t pid;
    char cmdline[1024];
} bg_job_t;

bg_job_t bg_jobs[MAX_BG_JOBS];
int bg_count = 0;

// Check for completed background jobs
void check_bg_jobs() {
    int status;
    for (int i = 0; i < bg_count; i++) {
        pid_t ret = waitpid(bg_jobs[i].pid, &status, WNOHANG);
        if (ret > 0) {
            printf("\n[Background Finished] PID: %d CMD: %s\n", bg_jobs[i].pid, bg_jobs[i].cmdline);
            // Remove from list
            for (int j = i; j < bg_count - 1; j++) {
                bg_jobs[j] = bg_jobs[j + 1];
            }
            bg_count--;
            i--;
        }
    }
}

// Add a new background job
void add_bg_job(pid_t pid, const char *cmdline) {
    if (bg_count < MAX_BG_JOBS) {
        bg_jobs[bg_count].pid = pid;
        strncpy(bg_jobs[bg_count].cmdline, cmdline, sizeof(bg_jobs[bg_count].cmdline) - 1);
        bg_jobs[bg_count].cmdline[sizeof(bg_jobs[bg_count].cmdline) - 1] = '\0';
        bg_count++;
    }
}

// Show all background jobs
void show_jobs() {
    for (int i = 0; i < bg_count; i++) {
        printf("[%d] PID: %d CMD: %s\n", i + 1, bg_jobs[i].pid, bg_jobs[i].cmdline);
    }
}

// Read input from user
char* read_cmd(const char *prompt) {
    char *line = NULL;
    size_t size = 0;
    printf("%s", prompt);
    ssize_t nread = getline(&line, &size, stdin);
    if (nread == -1) {
        free(line);
        return NULL;
    }
    // Remove trailing newline
    if (line[nread - 1] == '\n') line[nread - 1] = '\0';
    return line;
}

// Process a single line (handles ; and &)
void process_input(char *line) {
    if (!line) return;

    check_bg_jobs();  // reap zombies

    // Split by semicolon
    char *cmd_str = strtok(line, ";");
    while (cmd_str) {
        int background = 0;
        size_t len = strlen(cmd_str);

        // Check for background &
        if (len > 0 && cmd_str[len - 1] == '&') {
            background = 1;
            cmd_str[len - 1] = '\0';
        }

        // Tokenize
        command_t commands[MAX_COMMANDS];
        int num_commands = tokenize(cmd_str, commands);

        for (int i = 0; i < num_commands; i++) {
            // Handle shell variables assignment
            for (int j = 0; j < commands[i].argc; j++) {
                char *eq = strchr(commands[i].args[j], '=');
                if (eq) {
                    *eq = '\0';
                    set_variable(commands[i].args[j], eq + 1);
                    *eq = '=';  // restore
                    commands[i].args[j] = NULL;  // skip execution
                } else if (commands[i].args[j][0] == '$') {
                    char *val = get_variable(commands[i].args[j] + 1);
                    if (val) commands[i].args[j] = val;
                }
            }

            // Built-in commands
            if (commands[i].argc > 0 && strcmp(commands[i].args[0], "jobs") == 0) {
                show_jobs();
            } else if (commands[i].argc > 0 && strcmp(commands[i].args[0], "set") == 0) {
                print_all_variables();
            } else if (commands[i].argc > 0 && strcmp(commands[i].args[0], "exit") == 0) {
                printf("Bye!\n");
                exit(0);
            } else {
                execute_command(&commands[i], background);
            }
        }

        cmd_str = strtok(NULL, ";");
    }
    free(line);
}
