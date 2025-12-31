#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "ast.h"

typedef struct {
    Lexer *lexer;
    Token *cur_token;
    Token *peek_token;
    int error_count;
} Parser;

Parser* init_parser(Lexer *lexer);
Program* parse_program(Parser *parser);
void free_parser(Parser *parser);

#endif
