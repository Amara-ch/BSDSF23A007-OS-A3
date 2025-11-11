#include "variables.h"

var_t *var_list = NULL;

// Set or update variable
void set_variable(const char *name, const char *value) {
    var_t *curr = var_list;
    while (curr) {
        if (strcmp(curr->name, name) == 0) {
            free(curr->value);
            curr->value = strdup(value);
            return;
        }
        curr = curr->next;
    }
    var_t *new_var = malloc(sizeof(var_t));
    new_var->name = strdup(name);
    new_var->value = strdup(value);
    new_var->next = var_list;
    var_list = new_var;
}

// Get variable value
const char* get_variable(const char *name) {
    var_t *curr = var_list;
    while (curr) {
        if (strcmp(curr->name, name) == 0) {
            return curr->value;
        }
        curr = curr->next;
    }
    return NULL;
}

// Print all variables
void print_variables() {
    var_t *curr = var_list;
    while (curr) {
        printf("%s=%s\n", curr->name, curr->value);
        curr = curr->next;
    }
}
