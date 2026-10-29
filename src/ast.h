/**
 * @file ast.h
 * @internal Part of IFJ-project-2026-compiler, Team 274855
 *
 * @brief Header file for Abstract Syntax Tree building
 * @author Jakub Dufek (274855)
 * @author Ondřej Kycl (xkyclon00)
 * @date 3. 10. 2026
 */

#ifndef AST_H
#define AST_H

#include <stdbool.h>
#include <string.h>
#include "symtable.h"
#include "structures.h"

typedef struct ast_node ast_node_t;
typedef struct ast_block_node ast_block_node_t;


//  ENUM TYPES

/**
 * @enum ast_stmt_type_t
 * @brief Enum for statement types of nodes
 */
typedef enum ast_stmt_type {
    AST_BLOCK,
    AST_PARAM,

    AST_DECLARATION,
    AST_DEFINITION,

    EXPR_OPERATION,
    EXPR_IDENTIFIER,
    EXPR_LITERAL,

    EXPR_INT2FLOAT
} ast_stmt_type_t;



//  INDIVIDUAL NODE TYPES

/**
 * @struct ast_block_node_t
 * @brief Node representing block
 */
struct ast_block_node {
    ast_node_t *first;  /// Unit literal if empty block
    ast_node_t *last;   /// for insersion speed purposes ( O(1) instead of O(n) )
};

/**
 * @struct ast_param_node_t
 * @brief Parameter node
 */
typedef struct {
    char id_val[MAX_LENGTH];
    token_type data_type;
} ast_param_node_t;



/**
 * @struct ast_declaration_node_t
 * @brief Node for variable declaration
 */
typedef struct {
    char id_val[MAX_LENGTH];
    bool is_mutable;
    token_type data_type;
    ast_node_t *expression;     /// expression subtree
} ast_declaration_node_t;

/**
 * @struct ast_definition_node_t
 * @brief Node for function definition
 */
typedef struct {
    char id_val[MAX_LENGTH];
    token_type data_type;
    ast_node_t *first_param;    /// param->next is next param, if NULL no param
    ast_node_t *block;
    bool is_main;
} ast_definition_node_t;



/**
 * @struct expr_op_node_t
 * @brief Node for operators in expression module
 */
typedef struct {
    token_type op_type;
    ast_node_t *left;
    ast_node_t *right;
} expr_op_node_t;

/**
 * @struct expr_id_leaf_node_t
 * @brief Node for AST leaf - identifier
 */
typedef struct {
    char id_val[MAX_LENGTH];
    symbol_data_t symbol_data;
} expr_id_leaf_node_t;

/**
 * @struct expr_lit_leaf_node_t
 * @brief Node for AST leaf - literal
 */
typedef struct {
    token_type lit_type;
    token_value_t lit_value;
} expr_lit_leaf_node_t;

/**
 * @struct expr_imp_leaf_node_t
 * @brief Node for implicit conversion
 */
typedef struct {
    ast_node_t *operand;    /// pointer to converted operand
} expr_imp_conversion_t;



//  MAIN NODE TYPES

/**
 * @union ast_node_data_t
 * @brief Union of different nodes for different types of expressions
 */
typedef union {
    ast_block_node_t block_node;
    ast_param_node_t param_node;

    ast_declaration_node_t declaration_node;
    ast_definition_node_t definition_node;

    expr_op_node_t op_node;
    expr_id_leaf_node_t id_leaf_node;
    expr_lit_leaf_node_t lit_leaf_node;
    
    expr_imp_conversion_t imp_conversion;
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



//  HELPER FUNCTIONS

/**
 * @brief Creates subnode in ast for implicit conversion
 * 
 * @param inner_node 
 * 
 * @return New subnode which contains the new conversion and the operand for conversion
 */
ast_node_t *ast_convert_int2float(ast_node_t *inner_node);



//  AST INSERT FUNCTIONS

/**
 * @brief Creates node for new block
 * 
 * @param first ptr to first stmt
 * @param line_num
 * @param last ptr to last stmt (for insertion purposes)
 * 
 * @return AST node representing block
 */
ast_node_t *ast_insert_block(ast_node_t *first, unsigned line_num, ast_node_t *last);

/**
 * @brief Creates AST for function parameter
 * 
 * @param value name of id
 * @param data_type
 * @param next param right after
 * 
 * @return AST node representing param list
 */
ast_node_t *ast_insert_param(char value[MAX_LENGTH], unsigned line_num, token_type data_type, ast_node_t *next);



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
ast_node_t *ast_insert_var_declaration(char value[MAX_LENGTH], unsigned line_num, bool is_mutable,
                                            token_type data_type, ast_node_t *expression);

/**
 * @brief Creates AST for function definition
 * 
 * @param value
 * @param line_num
 * @param data_type
 * @param block
 * @param first_param
 * 
 * @return AST node representing function definition
 */
ast_node_t *ast_insert_fn_definition(char value[MAX_LENGTH], unsigned line_num, token_type data_type,
                                        ast_node_t *block, ast_node_t *first_param, bool is_main);



//  EXPR INSERT FUNCTIONS

/**
 * @brief Creates node for given operation in expr AST
 * 
 * @param opr enum statement type for which operation to insert
 * @param line_num
 * @param left node of left child in AST
 * @param right node of right child in AST
 * 
 * @return Allocated node for set operation
 */
ast_node_t *expr_insert_op(token_type opr, unsigned line_num, ast_node_t *left, ast_node_t *right);


/**
 * @brief Creates leaf node with identifier
 * 
 * @param value identifier key
 * @param line_num
 * 
 * @return Allocated node for given identifier
 */
ast_node_t *expr_insert_id_leaf(char value[MAX_LENGTH], unsigned line_num);


/**
 * @brief Creates leaf node with given literal
 * 
 * @param lit_value value of literal
 * @param line_num
 * @param lit_type type of literal (T_INT_LITERAL, T_UNIT_LITERAL, ...)
 * 
 * @return Allocated node for given literal
 */
ast_node_t *expr_insert_lit_leaf(token_value_t lit_value, unsigned line_num, token_type lit_type);


#endif
