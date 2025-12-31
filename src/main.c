#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include "parser.h"
#include "evaluator.h"

void run(char *input) {
    Lexer *lexer = init_lexer(input);
    Parser *parser = init_parser(lexer);
    Program *program = parse_program(parser);

    if (parser->error_count > 0) {
        fprintf(stderr, "Parsing failed with %d errors.\n", parser->error_count);
    } else {
        Environment *env = init_env(NULL);
        eval_program(program, env);
        fflush(stdout);
        free_environment(env);
    }

    free_program(program);
    free_parser(parser);
    free_lexer(lexer);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: aroy <filename>\n");
        return 1;
    }

    char *filename = argv[1];
    char *dot = strrchr(filename, '.');
    if (!dot || strcmp(dot, ".ar") != 0) {
        printf("Error: File must have .ar extension\n");
        return 1;
    }

    
    FILE *file = fopen(argv[1], "r");
    if (!file) {
        printf("Could not open file %s\n", argv[1]);
        return 1;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *buffer = malloc(length + 1);
    size_t read_size = fread(buffer, 1, length, file);
    buffer[read_size] = '\0';
    fclose(file);

    run(buffer);
    free(buffer);

    return 0;
}
