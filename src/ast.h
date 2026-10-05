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

#include <stdbool.h>

#include "structures.h" // MAX_LENGTH

typedef struct ast_node ast_node_t;



//  ENUM TYPES

/**
 * @enum ast_data_type_t
 * @brief Enum for data types in nodes
 */
typedef enum ast_data_type {
    AST_NO_T = 0,   /// no type given

    AST_INT_T,
    AST_DOUBLE_T,
    AST_STRING_T,
    AST_UNIT_T,

    AST_BOOL_T      /// BOOLTHEN extension
} ast_data_type_t;

/**
 * @enum ast_stmt_type_t
 * @brief Enum for statement types of nodes
 */
typedef enum ast_stmt_type {
    AST_DECLARATION
    // TODO: other types
} ast_stmt_type_t;



//  INDIVIDUAL NODE TYPES

/**
 * @struct ast_declaration_node_t
 * @brief Node for variable declaration
 */
typedef struct {
    char id_val[MAX_LENGTH];
    bool is_mutable;
    ast_data_type_t data_type;
    ast_node_t *expression;     /// another node (abstract tree)
} ast_declaration_node_t;



//  MAIN NODE TYPES

/**
 * @union ast_node_data_t
 * @brief Union of different nodes for different types of expressions
 */
typedef union {
    ast_declaration_node_t declaration_node;
    // TODO: other structures
} ast_node_data_t;

/**
 * @struct ast_node_t
 * @brief Main node of AST
 */
struct ast_node {
    ast_stmt_type_t stmt_type;
    unsigned line_num;
    ast_node_data_t *data;
    ast_node_t *next;
};



//  AST FUNCTIONS

/**
 * TODO: comment
 */
void ast_insert_var_declaration(char value[MAX_LENGTH], bool is_mutable, ast_data_type_t data_type, ast_node_t *expression);



#endif
