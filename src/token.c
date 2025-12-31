#include "token.h"
#include <stdlib.h>
#include <string.h>

Token* init_token(TokenType type, char *literal, int line, int column) {
    Token *token = malloc(sizeof(Token));
    token->type = type;
    token->literal = strdup(literal);
    token->line = line;
    token->column = column;
    return token;
}

void free_token(Token *token) {
    if (token->literal) {
        free(token->literal);
    }
    free(token);
}

const char* token_type_to_string(TokenType type) {
    switch (type) {
        case TOKEN_EOF: return "EOF";
        case TOKEN_INT: return "INT";
        case TOKEN_PLUS: return "PLUS";
        case TOKEN_MINUS: return "MINUS";
        case TOKEN_STAR: return "STAR";
        case TOKEN_SLASH: return "SLASH";
        case TOKEN_LBRACE: return "LBRACE";
        case TOKEN_RBRACE: return "RBRACE";
        case TOKEN_LPAREN: return "LPAREN";
        case TOKEN_RPAREN: return "RPAREN";
        case TOKEN_NEWLINE: return "NEWLINE";
        case TOKEN_ASSIGN: return "ASSIGN";
        case TOKEN_WRITE: return "WRITE";
        case TOKEN_INT_KEYWORD: return "INT_KEYWORD";
        case TOKEN_IDENT: return "IDENT";
        case TOKEN_ILLEGAL: return "ILLEGAL";
        default: return "UNKNOWN";
    }
}
