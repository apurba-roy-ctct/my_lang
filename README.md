# Aroy Programming Language

Aroy is a simple, interpreted programming language built from scratch in C. It features a clean syntax with newline termination, block scoping, and error handling.

## Features

- **Clean Syntax**: No semicolons required. Statements are terminated by newlines.
- **Data Types**: Support for Integers.
- **Variables**: Strong typing for variable declarations (`int`).
- **Arithmetic**: Full support for `+`, `-`, `*`, `/` with operator precedence and parentheses.
- **Scopes**: Block scoping using `{ ... }`. Inner blocks can access outer variables.
- **Comments**: Single-line comments using `//`.
- **Error Handling**: Detailed syntax and runtime error reporting with line numbers.

## Getting Started

### Prerequisites

- A C compiler (GCC recommended)
- Make (optional)

### Compilation

You can compile the interpreter using GCC:

```bash
gcc -I include -o aroy src/main.c src/lexer.c src/parser.c src/evaluator.c src/token.c src/ast.c
```

### Running Programs

Create a file with the `.ar` extension (e.g., `script.ar`) and run it using the interpreter:

```bash
./aroy script.ar
```

## Syntax Guide

### Output
Use the `write` keyword to print values or expressions to the console.

```aroy
write 123
write "Hello" // (Strings not yet implemented, only integers supported)
```

### Variables
Variables must be declared with the `int` keyword.

```aroy
int x = 10
int y = 20
write x + y
```

### Arithmetic
Standard arithmetic operators are supported.

```aroy
write 10 + 5 * 2   // 20
write (10 + 5) * 2 // 30
```

### Scopes
Variables declared inside a block `{}` are local to that block.

```aroy
int x = 10
{
    int z = 5
    write x + z // 15
}
// write z // Error: Undefined variable 'z'
```

### Comments
Use `//` for single-line comments.

```aroy
// This is a comment
int x = 5 // Inline comment
```

## Project Structure

- `src/`: Source code for the interpreter (Lexer, Parser, Evaluator).
- `include/`: Header files.
- `examples/`: Example Aroy scripts (`features.ar`, `errors.ar`).

## Error Handling

Aroy provides descriptive error messages for both syntax and runtime issues.

**Syntax Error:**
```
[Line 5] Error: Expected ')', found 'NEWLINE'
```

**Runtime Error:**
```
Error: Division by zero
Error: Undefined variable 'unknown_var'
```
