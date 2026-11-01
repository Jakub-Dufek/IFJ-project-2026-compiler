/**
 * @file main.c
 * @internal Part of IFJ-project-2026-compiler, Team 274855
 *
 * @brief File for main() function
 * @author Jakub Dufek (274855)
 * @date 17. 10. 2026
 */

#include "memory.h"
#include "parser.h"

int main()
{
    ast_init_arena();

    ast_free_arena();
}
