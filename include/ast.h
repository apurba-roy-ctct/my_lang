#ifndef AST_H
#define AST_H

#include "token.h"

typedef enum {
    NODE_PROGRAM,
    NODE_BLOCK_STATEMENT,
    NODE_VAR_DECLARATION,
    NODE_WRITE_STATEMENT,
    NODE_INFIX_EXPRESSION,
    NODE_PREFIX_EXPRESSION,
    NODE_INTEGER_LITERAL,
    NODE_IDENTIFIER
} NodeType;

typedef struct Node {
    NodeType type;
    struct Node *next; // For lists of statements
} Node;

typedef struct {
    Node base;
    Node *statements; // List of statements
} Program;

typedef struct {
    Node base;
    Node *statements;
} BlockStatement;

typedef struct {
    Node base;
    Node *expression;
} WriteStatement;

typedef struct {
    Node base;
    Token *token; // The 'int' token
    char *name;
    Node *value;
} VarDeclaration;

typedef struct {
    Node base;
    Token *token; // The identifier token
    char *value;
} Identifier;

typedef struct {
    Node base;
    Token *token; // The operator token, e.g. -
    char *operator;
    Node *right;
} PrefixExpression;

typedef struct {
    Node base;
    Token *token; // The operator token, e.g. +
    Node *left;
    Node *right;
    char *operator;
} InfixExpression;

typedef struct {
    Node base;
    Token *token;
    int value;
} IntegerLiteral;

// Constructors
Program* init_program();
BlockStatement* init_block_statement();
VarDeclaration* init_var_declaration(Token *token, char *name, Node *value);
WriteStatement* init_write_statement(Node *expression);
InfixExpression* init_infix_expression(Token *token, Node *left, char *operator, Node *right);
PrefixExpression* init_prefix_expression(Token *token, char *operator, Node *right);
IntegerLiteral* init_integer_literal(Token *token, int value);
Identifier* init_identifier(Token *token, char *value);

void free_node(Node *node);
void free_program(Program *program);

#endif
