#include "simplang.h"

int pos = 0;
Token* tkns;

void match(TokenType expected) {
    if (tkns[pos].type == expected) pos++;
    else {
        printf("Error4: Syntax Error\nUnexpected token at line %d.\nHint: Your code has a typo, like writing \"cofee\" instead of \"coffee\".\n", tkns[pos].line);
        exit(1);
    }
}

ASTNode* create_node(ASTType type) {
    ASTNode* node = calloc(1, sizeof(ASTNode));
    node->type = type;
    node->children = malloc(sizeof(ASTNode*) * 100);
    node->capacity = 100;
    return node;
}

ASTNode* parse_expression() {
    ASTNode* node = create_node(AST_NUMBER); // Default
    if (tkns[pos].type == T_NUMBER) {
        node->type = AST_NUMBER;
        node->num_val = atof(tkns[pos++].value);
    } else if (tkns[pos].type == T_STRING) {
        node->type = AST_STRING;
        node->str_val = strdup(tkns[pos++].value);
    } else if (tkns[pos].type == T_VARIABLE) {
        node->type = AST_VARIABLE;
        node->name = strdup(tkns[pos++].value);
    }
    return node;
}

ASTNode* parse_statement() {
    // 変数代入: @var = (expr)
    if (tkns[pos].type == T_VARIABLE) {
        ASTNode* node = create_node(AST_ASSIGN);
        node->name = strdup(tkns[pos++].value);
        match(T_ASSIGN);
        match(T_LPAREN);
        node->children[0] = parse_expression();
        node->child_count = 1;
        match(T_RPAREN);
        return node;
    }
    // ループ: exec(expr)[statements]
    if (tkns[pos].type == T_EXEC) {
        pos++; // skip 'exec'
        ASTNode* node = create_node(AST_EXEC);
        match(T_LPAREN);
        node->condition = parse_expression(); // 実行回数
        match(T_RPAREN);
        match(T_LBRACKET);
        while (tkns[pos].type != T_RBRACKET && tkns[pos].type != T_EOF) {
            node->children[node->child_count++] = parse_statement();
        }
        match(T_RBRACKET);
        return node;
    }
    // 関数呼び出し: print(expr)
    if (tkns[pos].type == T_IDENTIFIER) {
        ASTNode* node = create_node(AST_CALL);
        node->name = strdup(tkns[pos++].value);
        match(T_LPAREN);
        while (tkns[pos].type != T_RPAREN && tkns[pos].type != T_EOF) {
            node->children[node->child_count++] = parse_expression();
        }
        match(T_RPAREN);
        return node;
    }
    pos++; // スキップして無限ループ防止
    return NULL;
}

ASTNode* parse(Token* tokens) {
    tkns = tokens;
    pos = 0;
    ASTNode* root = create_node(AST_PROGRAM);
    while (tkns[pos].type != T_EOF) {
        ASTNode* stmt = parse_statement();
        if (stmt) root->children[root->child_count++] = stmt;
    }
    return root;
}