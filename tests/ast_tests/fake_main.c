#include <stdio.h>
#include <stdbool.h>
#include "../../src/memory.h"
#include "../../src/ast.h"
#include "../../src/parser.h"
#include "../../src/semantic_anal.h"
#include "ast_print_module.h"

int main()
{
    arena_init();
    scratch_init();

    // 1. Parser + Scanner postaví AST ze stdin
    ast_node_t *ast = build_ast();

    printf("========================================\n");
    printf("         AST BEFORE SEMANTICS           \n");
    printf("========================================\n");
    if (ast != NULL) {
        ast_print(ast, 0);
    } else {
        printf("(NULL AST)\n");
    }

    // 2. Sémantická analýza (pokud selže a končí přes exit(), ukončí se program zde)
    run_sem_anal(ast);

    printf("\n========================================\n");
    printf("         AST AFTER SEMANTICS            \n");
    printf("========================================\n");
    if (ast != NULL) {
        ast_print(ast, 0);
    }

    scratch_free();
    arena_free_all();
}