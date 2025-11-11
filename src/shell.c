#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "shell.h"

static bg_job_t bg_jobs[MAX_BG_JOBS];
static int bg_count = 0;

/* Check and reap finished background jobs */
void check_bg_jobs() {
    int status;
    for (int i = 0; i < bg_count; ) {
        pid_t ret = waitpid(bg_jobs[i].pid, &status, WNOHANG);
        if (ret > 0) {
            printf("\n[Background finished] %s (PID %d)\n", bg_jobs[i].cmdline, bg_jobs[i].pid);
            for (int j = i; j < bg_count - 1; j++)
                bg_jobs[j] = bg_jobs[j + 1];
            bg_count--;
        } else {
            i++;
        }
    }
}

/* Add a job to bg list */
void add_bg_job(int pid, const char* cmdline) {
    if (bg_count < MAX_BG_JOBS) {
        bg_jobs[bg_count].pid = pid;
        strncpy(bg_jobs[bg_count].cmdline, cmdline, 255);
        bg_jobs[bg_count].cmdline[255] = '\0';
        bg_count++;
    }
}

/* Print active background jobs */
void print_bg_jobs() {
    printf("Active background jobs:\n");
    for (int i = 0; i < bg_count; i++)
        printf("[%d] %s\n", bg_jobs[i].pid, bg_jobs[i].cmdline);
}

/* readline wrapper */
char* read_cmd(const char* prompt, FILE* fp) {
    char* line = NULL;
    size_t size = 0;
    if (fp == stdin) {
        printf("%s", prompt);
        fflush(stdout);
    }
    if (getline(&line, &size, fp) == -1) {
        free(line);
        return NULL;
    }
    /* remove newline */
    line[strcspn(line, "\n")] = 0;
    return line;
}

/* Process input: handle ; and & */
void process_input(char *line) {
    if (!line) return;

    char *saveptr = NULL;
    char *cmd_str = strtok_r(line, ";", &saveptr);

    while (cmd_str != NULL) {
        int background = 0;
        size_t len = strlen(cmd_str);
        if (len > 0 && cmd_str[len - 1] == '&') {
            background = 1;
            cmd_str[len - 1] = '\0';
        }

        command_t commands[MAX_COMMANDS];
        int ncmds = tokenize(cmd_str, commands);

        pid_t pid = fork();
        if (pid == 0) {
            execute(commands, ncmds);
            exit(0);
        } else if (pid > 0) {
            if (background) {
                add_bg_job(pid, cmd_str);
                printf("[running in background] %s (PID %d)\n", cmd_str, pid);
            } else {
                waitpid(pid, NULL, 0);
            }
        }
        cmd_str = strtok_r(NULL, ";", &saveptr);
    }
}

/* main shell loop */
void shell_loop() {
    while (1) {
        check_bg_jobs();
        char *line = read_cmd("FCIT> ", stdin);
        if (!line) break;
        if (strcmp(line, "exit") == 0) {
            printf("Bye!\n");
            free(line);
            break;
        }
        if (strcmp(line, "jobs") == 0) {
            print_bg_jobs();
            free(line);
            continue;
        }
        process_input(line);
        free(line);
    }
}
