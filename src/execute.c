#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "shell.h"

void execute(char** arglist) {
    pid_t cpid = fork();
    if (cpid < 0) {
        perror("fork");
        return;
    }

    if (cpid == 0) {
        // Child process
        execvp(arglist[0], arglist);
        perror("execvp"); // only reached if exec fails
        exit(1);
    } else {
        // Parent process
        int status;
        waitpid(cpid, &status, 0);
    }
}
