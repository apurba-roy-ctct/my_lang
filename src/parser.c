#include "parser.h"
#include <stdlib.h>
#include <stdio.h>

void next_token_parser(Parser *parser) {
    parser->cur_token = parser->peek_token;
    parser->peek_token = next_token(parser->lexer);
}

void eat_token(Parser *parser) {
    if (parser->cur_token) {
        free_token(parser->cur_token);
    }
    next_token_parser(parser);
}

void free_parser(Parser *parser) {
    if (parser->cur_token) free_token(parser->cur_token);
    if (parser->peek_token) free_token(parser->peek_token);
    free(parser);
}


Parser* init_parser(Lexer *lexer) {
    Parser *parser = malloc(sizeof(Parser));
    parser->lexer = lexer;
    parser->cur_token = NULL;
    parser->peek_token = next_token(lexer);
    parser->error_count = 0;
    next_token_parser(parser);
    return parser;
}

// Forward declaration
Node* parse_expression(Parser *parser);

// Highest Precedence: integer literals, identifiers, prefix expressions, (leaf nodes) and parentheses 
Node* parse_factor(Parser *parser) {
    Node *left = NULL;

    if (parser->cur_token->type == TOKEN_INT) {
        left = (Node*)init_integer_literal(parser->cur_token, atoi(parser->cur_token->literal));
        next_token_parser(parser);
    } else if (parser->cur_token->type == TOKEN_IDENT) {
        left = (Node*)init_identifier(parser->cur_token, parser->cur_token->literal);
        next_token_parser(parser);
    } else if (parser->cur_token->type == TOKEN_MINUS) {
        Token *token = parser->cur_token;
        next_token_parser(parser);
        Node *right = parse_factor(parser);
        left = (Node*)init_prefix_expression(token, "-", right);
    } else if (parser->cur_token->type == TOKEN_LPAREN) {
        eat_token(parser); // skip '('
        left = parse_expression(parser);
        if (parser->cur_token->type != TOKEN_RPAREN) {
            fprintf(stderr, "[Line %d] Error: Expected ')', found '%s'\n", 
                parser->cur_token->line, token_type_to_string(parser->cur_token->type));
            parser->error_count++;
            return NULL;
        }
        eat_token(parser); // skip ')'
    } else {
        fprintf(stderr, "[Line %d] Error: Unexpected token in factor: %s\n", 
            parser->cur_token->line, token_type_to_string(parser->cur_token->type));
        parser->error_count++;
        return NULL;
    }
    return left;
}

// Higher Precedence *, /
Node* parse_term(Parser *parser) {
    Node *left = parse_factor(parser);

    while (parser->cur_token->type == TOKEN_STAR || parser->cur_token->type == TOKEN_SLASH) {
        Token *op_token = parser->cur_token;
        char *op = parser->cur_token->literal;
        next_token_parser(parser);
        
        Node *right = parse_factor(parser);
        left = (Node*)init_infix_expression(op_token, left, op, right);
    }
    return left;
}

// Lowest Precedence +, -
Node* parse_expression(Parser *parser) {
    Node *left = parse_term(parser);

    while (parser->cur_token->type == TOKEN_PLUS || parser->cur_token->type == TOKEN_MINUS) {
        Token *op_token = parser->cur_token;
        char *op = parser->cur_token->literal;
        next_token_parser(parser);
        
        Node *right = parse_term(parser);
        left = (Node*)init_infix_expression(op_token, left, op, right);
    }
    return left;
}

Node* parse_statement(Parser *parser) {
    if (parser->cur_token->type == TOKEN_WRITE) {
        eat_token(parser); // skip 'write'
        Node *expr = parse_expression(parser);
        
        if (parser->cur_token->type == TOKEN_NEWLINE) {
            eat_token(parser); // skip newline
        }

        return (Node*)init_write_statement(expr);
    } else if (parser->cur_token->type == TOKEN_INT_KEYWORD) {
        // int x = 5;
        eat_token(parser); // skip 'int'
        
        if (parser->cur_token->type != TOKEN_IDENT) {
            fprintf(stderr, "[Line %d] Error: Expected identifier after 'int', found '%s'\n", 
                parser->cur_token->line, token_type_to_string(parser->cur_token->type));
            parser->error_count++;
            return NULL;
        }
        
        Token *ident_token = parser->cur_token;
        char *name = parser->cur_token->literal;
        next_token_parser(parser); // skip identifier

        if (parser->cur_token->type != TOKEN_ASSIGN) {
            fprintf(stderr, "[Line %d] Error: Expected '=' after identifier, found '%s'\n", 
                parser->cur_token->line, token_type_to_string(parser->cur_token->type));
            parser->error_count++;
            return NULL;
        }
        eat_token(parser); // skip '='

        Node *value = parse_expression(parser);

        if (parser->cur_token->type == TOKEN_NEWLINE) {
            eat_token(parser); // skip newline
        }

        return (Node*)init_var_declaration(ident_token, name, value);
    } else if (parser->cur_token->type == TOKEN_LBRACE) {
        eat_token(parser); // skip '{'
        BlockStatement *block = init_block_statement();
        Node *current = NULL;

        while (parser->cur_token->type != TOKEN_RBRACE && parser->cur_token->type != TOKEN_EOF) {
            if (parser->cur_token->type == TOKEN_NEWLINE) {
                eat_token(parser); // Skip empty lines inside blocks
                continue;
            }
            Node *stmt = parse_statement(parser);
            if (stmt) {
                if (block->statements == NULL) {
                    block->statements = stmt;
                    current = stmt;
                } else {
                    current->next = stmt;
                    current = stmt;
                }
            }
        }

        if (parser->cur_token->type == TOKEN_RBRACE) {
            eat_token(parser); // skip '}'
        }
        return (Node*)block;
    }
    return NULL;
}

Program* parse_program(Parser *parser) {
    Program *program = init_program();
    Node *current = NULL;

    while (parser->cur_token->type != TOKEN_EOF) {
        if (parser->cur_token->type == TOKEN_NEWLINE) {
            eat_token(parser); // Skip empty lines
            continue;
        }
        Node *stmt = parse_statement(parser);
        if (stmt) {
            if (program->statements == NULL) {
                program->statements = stmt;
                current = stmt;
            } else {
                current->next = stmt;
                current = stmt;
            }
        } else {
            fprintf(stderr, "[Line %d] Error: Unexpected token '%s'\n", 
                parser->cur_token->line, token_type_to_string(parser->cur_token->type));
            parser->error_count++;
            eat_token(parser); // Skip unknown token to avoid infinite loop
        }
    }
    return program;
}
