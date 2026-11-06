/**
 * @file main.c
 * @internal Part of IFJ-project-2026-compiler, Team 274855
 *
 * @brief File for main() function
 * @author Jakub Dufek (274855)
 * @date 17. 10. 2026
 */

#include "memory.h"
#include "ast.h"
#include "parser.h"
#include "semantic_anal.h"
#include "IFJcode_generator.h"

int main()
{
    arena_init();
    scratch_init();


    ast_node_t *ast = build_ast();  // parser + scanner
    run_sem_anal(ast);              // semantic analysis
    generate_IFJcode26(ast);        // code generator


    scratch_free();
    arena_free_all();
}
