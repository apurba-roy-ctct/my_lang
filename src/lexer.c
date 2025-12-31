#include "lexer.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

Lexer* init_lexer(char *input) {
    Lexer *lexer = malloc(sizeof(Lexer));
    lexer->input = strdup(input);
    lexer->position = 0;
    lexer->read_position = 0;
    lexer->line = 1;
    lexer->column = 0;
    read_char(lexer);
    return lexer;
}

void read_char(Lexer *lexer) {
    if (lexer->ch == '\n') {
        lexer->line++;
        lexer->column = 0;
    } else {
        lexer->column++;
    }

    if (lexer->read_position >= (int)strlen(lexer->input)) {
        lexer->ch = 0;
    } else {
        lexer->ch = lexer->input[lexer->read_position];
    }
    lexer->position = lexer->read_position;
    lexer->read_position++;
}

void skip_whitespace(Lexer *lexer) {
    while (lexer->ch == ' ' || lexer->ch == '\t' || lexer->ch == '\r') {
        read_char(lexer);
    }
}

void skip_comment(Lexer *lexer) {
    if (lexer->ch == '/' && lexer->input[lexer->read_position] == '/') {
        while (lexer->ch != '\n' && lexer->ch != 0) {
            read_char(lexer);
        }
        skip_whitespace(lexer);
    }
}

char* read_number(Lexer *lexer) {
    int position = lexer->position;
    while (isdigit(lexer->ch)) {
        read_char(lexer);
    }
    int len = lexer->position - position;
    char *num = malloc(len + 1);
    strncpy(num, lexer->input + position, len);
    num[len] = '\0';
    return num;
}

char* read_identifier(Lexer *lexer) {
    int position = lexer->position;
    while (isalnum(lexer->ch) || lexer->ch == '_') {
        read_char(lexer);
    }
    int len = lexer->position - position;
    char *ident = malloc(len + 1);
    strncpy(ident, lexer->input + position, len);
    ident[len] = '\0';
    return ident;
}

TokenType lookup_ident(char *ident) {
    if (strcmp(ident, "write") == 0) {
        return TOKEN_WRITE;
    }
    if (strcmp(ident, "int") == 0) {
        return TOKEN_INT_KEYWORD;
    }
    return TOKEN_IDENT;
}

Token* next_token(Lexer *lexer) {
    Token *token;
    skip_whitespace(lexer);

    // Check for comments
    while (lexer->ch == '/' && lexer->input[lexer->read_position] == '/') {
        skip_comment(lexer);
    }

    int line = lexer->line;
    int col = lexer->column;

    switch (lexer->ch) {
        case '{':
            token = init_token(TOKEN_LBRACE, "{", line, col);
            break;
        case '}':
            token = init_token(TOKEN_RBRACE, "}", line, col);
            break;
        case '(':
            token = init_token(TOKEN_LPAREN, "(", line, col);
            break;
        case ')':
            token = init_token(TOKEN_RPAREN, ")", line, col);
            break;
        case '+':
            token = init_token(TOKEN_PLUS, "+", line, col);
            break;
        case '-':
            token = init_token(TOKEN_MINUS, "-", line, col);
            break;
        case '*':
            token = init_token(TOKEN_STAR, "*", line, col);
            break;
        case '=':
            token = init_token(TOKEN_ASSIGN, "=", line, col);
            break;
        case '/':
            token = init_token(TOKEN_SLASH, "/", line, col);
            break;
        case '\n':
            token = init_token(TOKEN_NEWLINE, "\\n", line, col);
            break;
        case 0:
            token = init_token(TOKEN_EOF, "", line, col);
            break;
        default:
            if (isdigit(lexer->ch)) {
                char *literal = read_number(lexer);
                token = init_token(TOKEN_INT, literal, line, col);
                free(literal);
                return token;
            } else if (isalpha(lexer->ch)) {
                char *literal = read_identifier(lexer);
                TokenType type = lookup_ident(literal);
                token = init_token(type, literal, line, col);
                free(literal);
                return token;
            } else {
                token = init_token(TOKEN_ILLEGAL, "", line, col);
            }
            break;
    }

    read_char(lexer);
    return token;
}

void free_lexer(Lexer *lexer) {
    free(lexer->input);
    free(lexer);
}
