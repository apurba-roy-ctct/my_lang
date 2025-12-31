#ifndef LEXER_H
#define LEXER_H

#include "token.h"

typedef struct {
    char *input;
    int position;      // current position in input (points to current char)
    int read_position; // current reading position in input (after current char)
    char ch;           // current char under examination
    int line;
    int column;
} Lexer;

Lexer* init_lexer(char *input);
void read_char(Lexer *lexer);
Token* next_token(Lexer *lexer);
void free_lexer(Lexer *lexer);

#endif
