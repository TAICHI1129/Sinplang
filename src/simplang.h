#ifndef SIMPLANG_H
#define SIMPLANG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

typedef enum {
    T_NUMBER, T_STRING, T_VARIABLE, T_IDENTIFIER,
    T_ASSIGN, T_LPAREN, T_RPAREN, T_LBRACKET, T_RBRACKET,
    T_EXEC, T_EOF
} TokenType;

typedef struct {
    TokenType type;
    char* value;
    int line;
} Token;

typedef enum {
    AST_PROGRAM, AST_NUMBER, AST_STRING, AST_VARIABLE, 
    AST_ASSIGN, AST_EXEC, AST_CALL
} ASTType;

typedef struct ASTNode {
    ASTType type;
    char* name;
    double num_val;
    char* str_val;
    struct ASTNode* condition;
    struct ASTNode** children;
    int child_count;
    int capacity;
} ASTNode;

typedef struct Value {
    enum { V_NUM, V_STR, V_NULL } type;
    double n;
    char* s;
} Value;

typedef struct Environment {
    char** names;
    Value** values;
    int count;
} Environment;

// プロトタイプ宣言
Token* tokenize(const char* src);
ASTNode* parse(Token* tokens);
Value* eval(ASTNode* node, Environment* env);
void native_sleep(int milliseconds); // アセンブリ実装

#endif