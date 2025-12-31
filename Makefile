CC = gcc
CFLAGS = -Iinclude -Wall -Wextra

SRCS = src/main.c src/lexer.c src/token.c src/parser.c src/ast.c src/evaluator.c
TARGET = aroy

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	del $(TARGET).exe
