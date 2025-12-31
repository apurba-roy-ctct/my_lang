#include "evaluator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Environment* init_env(Environment *outer) {
    Environment *env = malloc(sizeof(Environment));
    env->variables = NULL;
    env->outer = outer;
    return env;
}

void env_set(Environment *env, char *name, int value) {
    Variable *current = env->variables;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            current->value = value;
            return;
        }
        current = current->next;
    }
    
    // Add new variable
    Variable *new_var = malloc(sizeof(Variable));
    new_var->name = strdup(name);
    new_var->value = value;
    new_var->next = env->variables;
    env->variables = new_var;
}

int env_get(Environment *env, char *name) {
    Variable *current = env->variables;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            return current->value;
        }
        current = current->next;
    }

    if (env->outer != NULL) {
        return env_get(env->outer, name);
    }

    fprintf(stderr, "Error: Undefined variable '%s'\n", name);
    exit(1);
}

int eval_expression(Node *node, Environment *env) {
    if (node->type == NODE_INTEGER_LITERAL) {
        return ((IntegerLiteral*)node)->value;
    } else if (node->type == NODE_IDENTIFIER) {
        return env_get(env, ((Identifier*)node)->value);
    } else if (node->type == NODE_PREFIX_EXPRESSION) {
        PrefixExpression *prefix = (PrefixExpression*)node;
        int right = eval_expression(prefix->right, env);
        if (strcmp(prefix->operator, "-") == 0) return -right;
    } else if (node->type == NODE_INFIX_EXPRESSION) {
        InfixExpression *infix = (InfixExpression*)node;
        int left = eval_expression(infix->left, env);
        int right = eval_expression(infix->right, env);

        if (strcmp(infix->operator, "+") == 0) return left + right;
        if (strcmp(infix->operator, "-") == 0) return left - right;
        if (strcmp(infix->operator, "*") == 0) return left * right;
        if (strcmp(infix->operator, "/") == 0) {
            if (right == 0) {
                fprintf(stderr, "Error: Division by zero\n");
                exit(1);
            }
            return left / right;
        }
    }
    return 0;
}

void eval_statement(Node *node, Environment *env) {
    if (node->type == NODE_WRITE_STATEMENT) {
        WriteStatement *stmt = (WriteStatement*)node;
        int result = eval_expression(stmt->expression, env);
        printf("%d\n", result);
    } else if (node->type == NODE_VAR_DECLARATION) {
        VarDeclaration *stmt = (VarDeclaration*)node;
        int val = eval_expression(stmt->value, env);
        env_set(env, stmt->name, val);
    } else if (node->type == NODE_BLOCK_STATEMENT) {
        BlockStatement *block = (BlockStatement*)node;
        Environment *block_env = init_env(env); // Create new scope
        Node *stmt = block->statements;
        while (stmt) {
            eval_statement(stmt, block_env);
            stmt = stmt->next;
        }
        free_environment(block_env);
    }
}

void eval_program(Program *program, Environment *env) {
    Node *stmt = program->statements;
    while (stmt) {
        eval_statement(stmt, env);
        stmt = stmt->next;
    }
}

void free_environment(Environment *env) {
    if (env == NULL) return;

    Variable *current = env->variables;
    while (current != NULL) {
        Variable *next = current->next;
        free(current->name);
        free(current);
        current = next;
    }
    free(env);
}
