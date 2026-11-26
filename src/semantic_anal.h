/**
 * @file semantic_anal.h
 * @internal Part of IFJ-project-2026-compiler, Team 274855
 *
 * @brief Header file for the semantic analysis phase of the compiler
 * @author Ondřej Kycl (xkyclon00)
 * @date 17. 10. 2026
 *
 * @details This file contains declarations of functions used for
 * semantic analysis, including built-in function initialization,
 * pre-analysis, and the main semantic check
 */

#ifndef SEMANTIC_ANAL_H
#define SEMANTIC_ANAL_H

#include "ast.h"
#include "error.h"
#include "symtable.h"

typedef token_type data_type; // More convenient rename for semantic analysis

/**
 * @brief Main entry point for semantic analysis.
 *
 * Initializes the global symbol table stack, inserts built-in functions,
 * runs the pre-analysis (to load global function definitions), and then
 * executes the main semantic analysis.
 *
 * @param ast_node Pointer to the root of the AST.
 */
void run_sem_anal(ast_node_t *ast_node);

/**
 * @brief First pass through the AST.
 *
 * Finds all function definitions on the global level and inserts them
 * into the symbol table to allow forward declarations and recursive calls.
 * Also strictly checks the presence and signature of the @main function.
 *
 * @param ast_node Pointer to the root of the AST.
 * @param stack Pointer to the initialized symbol table stack.
 */
void sem_pre_anal(ast_node_t *ast_node, symtable_stack_t *stack);



/**
 * @brief Second pass through the AST.
 *
 * Recursively analyzes the tree, performs type deductions, type checking,
 * variable scope management, and returns the resulting data type of the
 * evaluated block or statement.
 *
    This function was split into several static functions with Gemini Pro 3.1,
    because it seemed to be a much better solution than one large switch across
    hundreds of lines.
    Prompt:
       "Split this function into several functions (each case must have a standalone function now)"
 *
 *
 * @param ast_node Pointer to the currently evaluated AST node.
 * @param stack Pointer to the symbol table stack.
 * @return data_type The resulting data type of the evaluated node or block.
 */
data_type sem_anal(ast_node_t *ast_node, symtable_stack_t *stack);

/**
 * @brief Inserts all IFJ26 built-in functions into the global symbol table.
 *
 * @param stack Pointer to the symbol table stack (global frame).
 */
void builtin_functions_insertion(symtable_stack_t *stack);

void print_insertion(symtable_stack_t *stack);
void readline_insertion(symtable_stack_t *stack);
void readint_insertion(symtable_stack_t *stack);
void readdouble_insertion(symtable_stack_t *stack);
void tostr_insertion(symtable_stack_t *stack);
void toint_insertion(symtable_stack_t *stack);
void strlength_insertion(symtable_stack_t *stack);
void substr_insertion(symtable_stack_t *stack);
void ord_insertion(symtable_stack_t *stack);
void chr_insertion(symtable_stack_t *stack);
void readboolean_insertion(symtable_stack_t *stack);

/**
 * @brief Safely frees all local symbol tables currently on the semantic stack.
 *
 * This function is meant to be called by error_exit() to prevent memory leaks
 */
void sem_anal_error_exit();

#endif