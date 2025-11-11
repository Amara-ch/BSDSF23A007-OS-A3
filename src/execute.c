#include "shell.h"

int execute(char* arglist[]) {
    pid_t cpid;
    int status;

    cpid = fork();
    if (cpid == -1) {
        perror("fork");
        return -1;
    } 
    else if (cpid == 0) {
        // Child process
        execvp(arglist[0], arglist);
        perror("execvp"); // Only reached if exec fails
        exit(1);
    } 
    else {
        // Parent process waits for child
        waitpid(cpid, &status, 0);
    }

    return 0;
}
