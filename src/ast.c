/**
 * @file ast.c
 * @internal Part of IFJ-project-2026-compiler, Team 274855
 *
 * @brief Source file for Abstract Syntax Tree building
 * @author Jakub Dufek (274855)
 * @author Ondřej Kycl (xkyclon00)
 * @date 3. 10. 2026
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdalign.h>

#include "ast.h"
#include "error.h"
#include "memory.h"



//  HELPER FUNCTIONS

/**
 * @brief Allocates data for one ast_node_t and it's "data" component
 * 
 * @param stmt_type type of node
 * @param line_num
 *
 * @return Initialized node ready to use
 */
static ast_node_t *ast_node_init(ast_stmt_type_t stmt_type, unsigned line_num)
{
    ast_node_t *node = ast_alloc(sizeof(ast_node_t));
    node->data = ast_alloc(sizeof(ast_node_data_t));
    node->next = NULL;

    node->stmt_type = stmt_type;
    node->line_num = line_num;

    return node;
}


ast_node_t *ast_convert_int2float(ast_node_t *inner_node) {
    ast_node_t *conv_node = ast_node_init(EXPR_INT2FLOAT, inner_node->line_num);

    conv_node->data->imp_conversion.operand = inner_node;

    return conv_node;
}



//  AST INSERT FUNCTIONS

ast_node_t *ast_insert_block(ast_node_t *first, unsigned line_num, ast_node_t *last)
{
    ast_node_t *current_node = ast_node_init(AST_BLOCK, line_num);

    current_node->data->block_node.first = first;
    current_node->data->block_node.last = last;

    return current_node;
}


ast_node_t *ast_insert_param(char value[MAX_LENGTH], unsigned line_num, token_type data_type, ast_node_t *next)
{
    ast_node_t *current_node = ast_node_init(AST_PARAM, line_num);

    current_node->next = next;
    current_node->data->param_node.data_type = data_type;
    strcpy(current_node->data->param_node.id_val, value);

    return current_node;
}



ast_node_t *ast_insert_var_declaration(char value[MAX_LENGTH], unsigned line_num, bool is_mutable,
                                            token_type data_type, ast_node_t *expression)
{
    ast_node_t *current_node = ast_node_init(AST_DECLARATION, line_num);

    strcpy(current_node->data->declaration_node.id_val, value);
    current_node->data->declaration_node.is_mutable = is_mutable;
    current_node->data->declaration_node.data_type = data_type;
    current_node->data->declaration_node.expression = expression;
    
    return current_node;
}


ast_node_t *ast_insert_fn_definition(char value[MAX_LENGTH], unsigned line_num, token_type data_type,
                                        ast_node_t *block, ast_node_t *first_param, bool is_main)
{
    ast_node_t *current_node = ast_node_init(AST_DEFINITION, line_num);

    strcpy(current_node->data->definition_node.id_val, value);
    current_node->data->definition_node.data_type = data_type;
    current_node->data->definition_node.first_param = first_param;
    current_node->data->definition_node.block = block;
    current_node->data->definition_node.is_main = is_main;

    return current_node;
}



//  EXPR INSERT FUNCTIONS

ast_node_t *expr_insert_op(token_type opr, unsigned line_num, ast_node_t *left, ast_node_t *right)
{
    ast_node_t *op_node = ast_node_init(EXPR_OPERATION, line_num);

    op_node->data->op_node.op_type = opr;
    op_node->data->op_node.left = left;
    op_node->data->op_node.right = right;

    return op_node;
}


ast_node_t *expr_insert_id_leaf(char value[MAX_LENGTH], unsigned line_num)
{
    ast_node_t *leaf_node = ast_node_init(EXPR_IDENTIFIER, line_num);
    strcpy(leaf_node->data->id_leaf_node.id_val, value);

    return leaf_node;
}


ast_node_t *expr_insert_lit_leaf(token_value_t lit_value, unsigned line_num, token_type lit_type)
{
    ast_node_t *leaf_node = ast_node_init(EXPR_LITERAL, line_num);

    leaf_node->data->lit_leaf_node.lit_type = lit_type;
    leaf_node->data->lit_leaf_node.lit_value = lit_value;

    return leaf_node;
}
