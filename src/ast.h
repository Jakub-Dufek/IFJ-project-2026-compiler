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
#include <string.h>

#include "structures.h" // MAX_LENGTH

#define ARENA_SIZE 1024 * 1024 // 1 MB

typedef struct ast_node ast_node_t;
typedef struct arena arena_t;


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
    AST_DECLARATION,

    /// operators falling under "expr_op_node_t"
    EXPR_ADD,   // '+'
    EXPR_SUB,   // '-'
    EXPR_MULT,  // '*'
    EXPR_DIV,   // '/'
    EXPR_GT,    // '>'
    EXPR_GTE,   // '>='
    EXPR_LT,    // '<'
    EXPR_LTE,   // '<='
    EXPR_EQ,    // '=='
    EXPR_NEQ,   // '!='

    /// leaf nodes
    EXPR_IDENTIFIER,
    EXPR_LITERAL
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

/**
 * @struct expr_op_node_t
 * @brief Node for operators in expression module
 */
typedef struct {
    ast_node_t *left;
    ast_node_t *right;
} expr_op_node_t;

/**
 * @struct expr_id_leaf_node_t
 * @brief Node for AST leaf - identifier
 */
typedef struct {
    char id_val[MAX_LENGTH];
} expr_id_leaf_node_t;

/**
 * @struct expr_lit_leaf_node_t
 * @brief Node for AST leaf - literal
 */
typedef struct {
    token_type lit_type;
    token_value_t lit_value;
} expr_lit_leaf_node_t;



//  MAIN NODE TYPES

/**
 * @union ast_node_data_t
 * @brief Union of different nodes for different types of expressions
 */
typedef union {
    ast_declaration_node_t declaration_node;

    expr_op_node_t op_node;
    expr_id_leaf_node_t id_leaf_node;
    expr_lit_leaf_node_t lit_leaf_node;
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
 * @brief Creates AST for var/val declaration (usually one line of code)
 * 
 * @param value Value of var/val (key)
 * @param is_mutable Mutability of var/val
 * @param data_type Data type given (or to be deduced)
 * @param expression a sub-AST of expression
 * 
 * @return AST node representing var/val declaration
 */
ast_node_t *ast_insert_var_declaration(char value[MAX_LENGTH], bool is_mutable, token_type data_type, ast_node_t *expression);



//  MEMORY HANDLING 

/**
 * @struct arena_t
 * @brief struct representing 1 block of memory (1 MB)
 */
struct arena {
    char arena[ARENA_SIZE];
    unsigned offset;
    arena_t *next;
};

/**
 * @brief TODO: comment
 */
static unsigned align_size(unsigned size);

/**
 * @brief Initializes arena for AST
 */
void ast_init_arena();


/**
 * @brief Allocates 1 block of memory (1 arena)
 * @return // TODO: specify, create struct
 */
static void *ast_alloc_node(unsigned size);



#endif
