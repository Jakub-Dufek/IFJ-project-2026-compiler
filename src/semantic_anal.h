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

//missing information 

//functions 
/*
 
*/
data_type sem_anal(ast_node_t *ast_node, symtable_bvs_node_t **symtable_root);

void run_sem_anal(ast_node_t *ast_node);
#endif