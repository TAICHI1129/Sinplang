#include "simplang.h"
#include <time.h>

// [アセンブリ実装] x86_64 Linux向け sys_nanosleep システムコールの直接呼び出し
void native_sleep(int milliseconds) {
    struct timespec req = { milliseconds / 1000, (milliseconds % 1000) * 1000000L };
    __asm__ volatile (
        "mov $35, %%rax\n"     // syscall番号 35 (sys_nanosleep)
        "mov %0, %%rdi\n"      // 第1引数: reqへのポインタ
        "xor %%rsi, %%rsi\n"   // 第2引数: NULL (remへのポインタ)
        "syscall"
        : 
        : "r" (&req)
        : "rax", "rdi", "rsi", "rcx", "r11", "memory"
    );
}

Environment global_env = {NULL, NULL, 0};

Value* env_get(char* name) {
    for (int i = 0; i < global_env.count; i++) {
        if (strcmp(global_env.names[i], name) == 0) return global_env.values[i];
    }
    printf("Error5: Variable Not Found\nVariable \"%s\" is not declared.\nHint: You're looking for your keys, but you never had them.\n", name);
    exit(1);
}

void env_set(char* name, Value* val) {
    for (int i = 0; i < global_env.count; i++) {
        if (strcmp(global_env.names[i], name) == 0) {
            global_env.values[i] = val; return;
        }
    }
    if (global_env.count == 0) {
        global_env.names = malloc(sizeof(char*) * 100);
        global_env.values = malloc(sizeof(Value*) * 100);
    }
    global_env.names[global_env.count] = strdup(name);
    global_env.values[global_env.count++] = val;
}

Value* eval(ASTNode* node, Environment* env) {
    if (!node) return NULL;
    Value* res = malloc(sizeof(Value));
    res->type = V_NULL;

    switch (node->type) {
        case AST_NUMBER: res->type = V_NUM; res->n = node->num_val; return res;
        case AST_STRING: res->type = V_STR; res->s = node->str_val; return res;
        case AST_VARIABLE: return env_get(node->name);
        case AST_ASSIGN:
            env_set(node->name, eval(node->children[0], env));
            break;
        case AST_EXEC: {
            Value* count = eval(node->condition, env);
            if (count->type != V_NUM) {
                printf("Error3: Type Error\nexec count must be a number.\nHint: Chicken can't be steak.\n");
                exit(1);
            }
            int loops = (int)count->n;
            for (int i = 0; i < loops; i++) {
                for (int j = 0; j < node->child_count; j++) {
                    eval(node->children[j], env);
                }
            }
            break;
        }
        case AST_CALL:
            if (strcmp(node->name, "print") == 0) {
                for (int i = 0; i < node->child_count; i++) {
                    Value* v = eval(node->children[i], env);
                    if (v->type == V_NUM) printf("%g", v->n);
                    else if (v->type == V_STR) printf("%s", v->s);
                }
                printf("\n");
            } else if (strcmp(node->name, "sleep") == 0) {
                Value* ms = eval(node->children[0], env);
                native_sleep((int)ms->n); // アセンブリ実装の呼び出し
            }
            break;
        case AST_PROGRAM:
            for (int i = 0; i < node->child_count; i++) eval(node->children[i], env);
            break;
    }
    return res;
}