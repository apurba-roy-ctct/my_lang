#include "ast.h"
#include <stdlib.h>
#include <string.h>

Program* init_program() {
    Program *program = malloc(sizeof(Program));
    program->base.type = NODE_PROGRAM;
    program->base.next = NULL;
    program->statements = NULL;
    return program;
}

BlockStatement* init_block_statement() {
    BlockStatement *stmt = malloc(sizeof(BlockStatement));
    stmt->base.type = NODE_BLOCK_STATEMENT;
    stmt->base.next = NULL;
    stmt->statements = NULL;
    return stmt;
}

VarDeclaration* init_var_declaration(Token *token, char *name, Node *value) {
    VarDeclaration *stmt = malloc(sizeof(VarDeclaration));
    stmt->base.type = NODE_VAR_DECLARATION;
    stmt->base.next = NULL;
    stmt->token = token;
    stmt->name = strdup(name);
    stmt->value = value;
    return stmt;
}

WriteStatement* init_write_statement(Node *expression) {
    WriteStatement *stmt = malloc(sizeof(WriteStatement));
    stmt->base.type = NODE_WRITE_STATEMENT;
    stmt->base.next = NULL;
    stmt->expression = expression;
    return stmt;
}

InfixExpression* init_infix_expression(Token *token, Node *left, char *operator, Node *right) {
    InfixExpression *expr = malloc(sizeof(InfixExpression));
    expr->base.type = NODE_INFIX_EXPRESSION;
    expr->base.next = NULL;
    expr->token = token;
    expr->left = left;
    expr->operator = strdup(operator);
    expr->right = right;
    return expr;
}

PrefixExpression* init_prefix_expression(Token *token, char *operator, Node *right) {
    PrefixExpression *expr = malloc(sizeof(PrefixExpression));
    expr->base.type = NODE_PREFIX_EXPRESSION;
    expr->base.next = NULL;
    expr->token = token;
    expr->operator = strdup(operator);
    expr->right = right;
    return expr;
}

IntegerLiteral* init_integer_literal(Token *token, int value) {
    IntegerLiteral *lit = malloc(sizeof(IntegerLiteral));
    lit->base.type = NODE_INTEGER_LITERAL;
    lit->base.next = NULL;
    lit->token = token;
    lit->value = value;
    return lit;
}

Identifier* init_identifier(Token *token, char *value) {
    Identifier *ident = malloc(sizeof(Identifier));
    ident->base.type = NODE_IDENTIFIER;
    ident->base.next = NULL;
    ident->token = token;
    ident->value = strdup(value);
    return ident;
}

void free_node(Node *node) {
    if (node == NULL) return;

    switch (node->type) {
        case NODE_PROGRAM:
            free_node(((Program*)node)->statements);
            break;
        case NODE_BLOCK_STATEMENT:
            free_node(((BlockStatement*)node)->statements);
            break;
        case NODE_VAR_DECLARATION: {
            VarDeclaration *stmt = (VarDeclaration*)node;
            free_token(stmt->token);
            free(stmt->name);
            free_node(stmt->value);
            break;
        }
        case NODE_WRITE_STATEMENT:
            free_node(((WriteStatement*)node)->expression);
            break;
        case NODE_INFIX_EXPRESSION: {
            InfixExpression *expr = (InfixExpression*)node;
            free_token(expr->token);
            free(expr->operator);
            free_node(expr->left);
            free_node(expr->right);
            break;
        }
        case NODE_PREFIX_EXPRESSION: {
            PrefixExpression *expr = (PrefixExpression*)node;
            free_token(expr->token);
            free(expr->operator);
            free_node(expr->right);
            break;
        }
        case NODE_INTEGER_LITERAL:
            free_token(((IntegerLiteral*)node)->token);
            break;
        case NODE_IDENTIFIER: {
            Identifier *ident = (Identifier*)node;
            free_token(ident->token);
            free(ident->value);
            break;
        }
    }

    // Free the next node in the list (if any)
    if (node->next) {
        free_node(node->next);
    }

    free(node);
}

void free_program(Program *program) {
    if (program) {
        free_node((Node*)program);
    }
}
