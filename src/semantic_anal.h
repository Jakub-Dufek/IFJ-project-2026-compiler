/**
 * @file semantic_anal.h
 * @internal Part of IFJ-project-2026-compiler, Team 274855
 *
 * @brief semantic analysis phase of the compiler
 * @author Ondřej Kycl (xkyclon00)
 * @date 17. 10. 2026
 * 
 * @details This file contains declarations and descriptions of functions used in semantic_anal.c
 */

#ifndef SEM_ANAL_H
#define SEM_ANAL_H

//libraries
#include "structures.h"
#include "ast.h"
#include "symtable.h"


typedef token_type data_type; // More convenient rename for semantic analysis


//functions 

/*
 Executes first go-through AST for gathering global function header information, 
 like return type, parameter types and also checking for entry function (@main)
*/
void sem_pre_anal(ast_node_t *ast_node, symtable_stack_t *stack);

/*
 Executes second go-through AST
*/
data_type sem_anal(ast_node_t *ast_node, symtable_stack_t *stack);

/*
 Calls all functions below inserting built-in functions into top level of the symtable
*/
void builtin_functions_insertion(symtable_stack_t *stack);

/*
 Built-in functions filled and inserted into symtable 
*/
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

/*
 Performs entire semantic analysis by creating and destroying symtable stack, 
 and calling functions above (builtin_functions_insertion, pre_sem_anal, sem_anal)
*/
void run_sem_anal(ast_node_t *ast_node);
#endif