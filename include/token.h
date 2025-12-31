#ifndef TOKEN_H
#define TOKEN_H

typedef enum {
    TOKEN_EOF,
    TOKEN_INT,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_STAR,
    TOKEN_SLASH,
    TOKEN_LBRACE, // {
    TOKEN_RBRACE, // }
    TOKEN_LPAREN, // (
    TOKEN_RPAREN, // )
    TOKEN_NEWLINE,// \n
    TOKEN_ASSIGN, // =
    TOKEN_WRITE,  // write keyword
    TOKEN_INT_KEYWORD, // int keyword
    TOKEN_IDENT,  // variable name
    TOKEN_ILLEGAL
} TokenType;

typedef struct {
    TokenType type;
    char *literal;
    int line;
    int column;
} Token;

Token* init_token(TokenType type, char *literal, int line, int column);
void free_token(Token *token);
const char* token_type_to_string(TokenType type);

#endif
