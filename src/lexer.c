#include "simplang.h"

Token* tokenize(const char* src) {
    Token* tokens = malloc(sizeof(Token) * 2000);
    int c = 0, i = 0, line = 1;

    while (src[i] != '\0') {
        if (src[i] == '\n') { line++; i++; continue; }
        if (isspace(src[i])) { i++; continue; }
        
        // コメント (--)
        if (src[i] == '-' && src[i+1] == '-') {
            while (src[i] != '\0' && src[i] != '\n') i++;
            continue;
        }

        if (src[i] == '@') {
            int start = i++;
            while (isalnum(src[i]) || src[i] == '_') i++;
            tokens[c].type = T_VARIABLE;
            tokens[c].line = line;
            tokens[c++].value = strndup(src + start, i - start);
            continue;
        }

        if (isdigit(src[i])) {
            int start = i;
            while (isdigit(src[i]) || src[i] == '.') i++;
            tokens[c].type = T_NUMBER;
            tokens[c].line = line;
            tokens[c++].value = strndup(src + start, i - start);
            continue;
        }

        if (src[i] == '"') {
            int start = ++i;
            while (src[i] != '"' && src[i] != '\0') i++;
            tokens[c].type = T_STRING;
            tokens[c].line = line;
            tokens[c++].value = strndup(src + start, i - start);
            i++; continue;
        }

        if (isalpha(src[i])) {
            int start = i;
            while (isalnum(src[i]) || src[i] == '_') i++;
            char* word = strndup(src + start, i - start);
            tokens[c].line = line;
            if (strcmp(word, "exec") == 0) tokens[c].type = T_EXEC;
            else tokens[c].type = T_IDENTIFIER;
            tokens[c++].value = word;
            continue;
        }

        tokens[c].line = line;
        if (src[i] == '=') { tokens[c++].type = T_ASSIGN; }
        else if (src[i] == '(') { tokens[c++].type = T_LPAREN; }
        else if (src[i] == ')') { tokens[c++].type = T_RPAREN; }
        else if (src[i] == '[') { tokens[c++].type = T_LBRACKET; }
        else if (src[i] == ']') { tokens[c++].type = T_RBRACKET; }
        i++;
    }
    tokens[c].type = T_EOF;
    return tokens;
}