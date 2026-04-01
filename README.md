# Sinplang
A simple and fun programming language. Enjoy coding!!
Simplang is a minimalist, Turing-complete programming language designed for both simplicity and low-level performance. It features a custom recursive descent parser written in C and leverages x86_64 inline assembly for direct OS system calls.

## Key Features

- **Strict Grammar**: Defined via LARK (EBNF-based) for formal verification.
- **Native Execution**: Core interpreter implemented in C11.
- **Low-Level Integration**: Direct system calls (e.g., `sys_nanosleep`) using x86_64 inline assembly.
- **Recursive Scoping**: Support for dynamic variable environments.
- **Turing Complete**: Includes conditional branching, `exec` loops, and recursive function support.

## Project Structure

- `src/lexer.c`: Tokenizes raw string input into a stream of typed tokens.
- `src/parser.c`: Constructs an Abstract Syntax Tree (AST) using a recursive descent algorithm.
- `src/interpreter.c`: Traverses the AST and executes logic, including assembly-level optimizations.
- `simplang.lark`: The formal grammar definition for the Simplang language.

## Syntax Overview

### Variable Assignment
```simplang
@x = (42)
@msg = ("Hello, Simplang")