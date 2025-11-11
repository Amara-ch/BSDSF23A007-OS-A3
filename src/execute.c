#include "shell.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>

// Execute commands with I/O redirection and pipes
void execute(command_t *commands, int ncmds) {
    int i;
    int pipefd[2*ncmds]; // pipefds for n-1 pipes

    for (i = 0; i < ncmds - 1; i++) {
        if (pipe(pipefd + i*2) < 0) {
            perror("pipe");
            exit(1);
        }
    }

    int cmd_count = 0;
    for (i = 0; i < ncmds; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            // Input redirection
            if (commands[i].input_file) {
                int fd = open(commands[i].input_file, O_RDONLY);
                if (fd < 0) { perror("open input"); exit(1); }
                dup2(fd, STDIN_FILENO);
                close(fd);
            }

            // Output redirection
            if (commands[i].output_file) {
                int fd = open(commands[i].output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (fd < 0) { perror("open output"); exit(1); }
                dup2(fd, STDOUT_FILENO);
                close(fd);
            }

            // Set up pipes
            if (i != 0) {
                dup2(pipefd[(i-1)*2], STDIN_FILENO);
            }
            if (i != ncmds-1) {
                dup2(pipefd[i*2 + 1], STDOUT_FILENO);
            }

            // Close all pipe fds in child
            for (int j = 0; j < 2*(ncmds-1); j++)
                close(pipefd[j]);

            execvp(commands[i].argv[0], commands[i].argv);
            perror("execvp");
            exit(1);
        } else if (pid < 0) {
            perror("fork");
            exit(1);
        }
    }

    // Parent closes all pipes
    for (i = 0; i < 2*(ncmds-1); i++)
        close(pipefd[i]);

    // Parent waits for all children
    for (i = 0; i < ncmds; i++)
        wait(NULL);
}
