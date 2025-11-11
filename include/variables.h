#ifndef VARIABLES_H
#define VARIABLES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct var_s {
    char *name;
    char *value;
    struct var_s *next;
} var_t;

// Head of linked list
extern var_t *var_list;

// Functions
void set_variable(const char *name, const char *value);
const char* get_variable(const char *name);
void print_variables();

#endif
