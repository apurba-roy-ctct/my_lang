#ifndef EVALUATOR_H
#define EVALUATOR_H

#include "ast.h"

typedef struct Variable {
    char *name;
    int value;
    struct Variable *next;
} Variable;

typedef struct Environment {
    Variable *variables;
    struct Environment *outer;
} Environment;

Environment* init_env(Environment *outer);
void env_set(Environment *env, char *name, int value);
int env_get(Environment *env, char *name);
void free_environment(Environment *env);

void eval_program(Program *program, Environment *env);

#endif
