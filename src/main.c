#include "simplang.h"

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Usage: simplang <file.simp>\n");
        return 1;
    }

    FILE* file = fopen(argv[1], "rb");
    if (!file) {
        printf("Error9: File Not Found\nHint: The treasure map leads to a place that doesn't exist.\n");
        return 1;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);
    char* source = malloc(length + 1);
    fread(source, 1, length, file);
    source[length] = '\0';
    fclose(file);

    Token* tokens = tokenize(source);
    ASTNode* ast = parse(tokens);
    eval(ast, &global_env);

    free(source);
    return 0;
}