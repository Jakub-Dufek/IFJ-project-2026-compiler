/**
 * @file ast.h
 * @internal Part of IFJ-project-2026-compiler, Team 274855
 *
 * @brief Header file for Abstract Syntax Tree building
 * @author Jakub Dufek (274855)
 * @date 3. 10. 2026
 */

#ifndef AST_H
#define AST_H

#include "structures.h" // MAX_LENGTH

typedef struct ast_node ast_node_t;


//  INDIVIDUAL NODE TYPES

/**
 * TODO: comment
 */
typedef struct {
    char id_val[MAX_LENGTH];
    bool is_mutable;
    int data_type; //TODO: enum
    ast_node_t *expression;
} ast_declaration_node_t;


//  MAIN NODE TYPES

/**
 * @brief Union of different nodes for different types of expressions
 */
typedef union {
    ast_declaration_node_t declaration_node;
    // TODO: other structures
} ast_node_data_t;

/**
 * @brief Main node of AST
 */
struct ast_node {
    int type;       //TODO: enum
    unsigned line_num;
    ast_node_data_t *data;
    ast_node_t *next;
};

/**
 * TODO: comment
 */
void ast_insert_var_declaration();


#endif
