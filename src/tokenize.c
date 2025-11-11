// Detect assignment
for (int i = 0; i < cmd->argc; i++) {
    char *eq = strchr(cmd->args[i], '=');
    if (eq && eq != cmd->args[i]) { // valid assignment (not starting with '=')
        *eq = '\0';
        set_variable(cmd->args[i], eq + 1);
        // Remove assignment from args
        free(cmd->args[i]);
        for (int j = i; j < cmd->argc - 1; j++) {
            cmd->args[j] = cmd->args[j + 1];
        }
        cmd->argc--;
        i--; // check current index again
    }
}
