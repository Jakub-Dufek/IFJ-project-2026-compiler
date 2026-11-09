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
#include <string.h>

#include "ast.h"
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
    ast_node_t *node = arena_alloc(sizeof(ast_node_t));
    node->data = arena_alloc(sizeof(ast_node_data_t));
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



//  DISCRETE INSERT FUNCTIONS

ast_node_t *ast_insert_block(ast_node_t *first, unsigned line_num, ast_node_t *last)
{
    ast_node_t *current_node = ast_node_init(AST_BLOCK, line_num);

    current_node->data->block_node.first = first;
    current_node->data->block_node.last = last;

    return current_node;
}


ast_node_t *ast_insert_param(char *value, unsigned line_num, token_type data_type, ast_node_t *next)
{
    ast_node_t *current_node = ast_node_init(AST_PARAM, line_num);

    current_node->next = next;
    current_node->data->param_node.data_type = data_type;
    current_node->data->param_node.id_val = value;

    return current_node;
}



//  LOCAL INSERT FUNCTIONS

ast_node_t *ast_insert_var_declaration(char *value, unsigned line_num, bool is_mutable,
                                            token_type data_type, ast_node_t *expr_or_call)
{
    ast_node_t *current_node = ast_node_init(AST_DECLARATION, line_num);

    current_node->data->declaration_node.id_val = value;
    current_node->data->declaration_node.is_mutable = is_mutable;
    current_node->data->declaration_node.data_type = data_type;
    current_node->data->declaration_node.expr_or_call = expr_or_call;
    
    return current_node;
}


ast_node_t *ast_insert_assign(char *value, unsigned line_num, ast_node_t *expr_or_call)
{
    ast_node_t *current_node = ast_node_init(AST_ASSIGN, line_num);

    current_node->data->assign_node.id_val = value;
    current_node->data->assign_node.expr_or_call = expr_or_call;

    return current_node;
}


ast_node_t *ast_insert_expr(ast_node_t *expression, unsigned line_num)
{
    ast_node_t *current_node = ast_node_init(AST_EXPR, line_num);
    current_node->data->expr_node.expression = expression;

    return current_node;
}


ast_node_t *ast_insert_call(char *value, unsigned line_num, ast_node_t *first_arg)
{
    ast_node_t *current_node = ast_node_init(AST_CALL, line_num);

    current_node->data->call_node.id_val = value;
    current_node->data->call_node.first_arg = first_arg;

    return current_node;

}


ast_node_t *ast_insert_if(ast_node_t *condition, unsigned line_num, ast_node_t *then_body, ast_node_t *else_body)
{
    ast_node_t *current_node = ast_node_init(AST_IF, line_num);

    current_node->data->if_node.condition = condition;
    current_node->data->if_node.then_body = then_body;
    current_node->data->if_node.else_body = else_body;

    return current_node;
}


ast_node_t *ast_insert_while(ast_node_t *condition, unsigned line_num, ast_node_t *body)
{
    ast_node_t *current_node = ast_node_init(AST_WHILE, line_num);

    current_node->data->while_node.condition = condition;
    current_node->data->while_node.body = body;

    return current_node;
}


ast_node_t *ast_insert_return(ast_node_t *expr_or_call, unsigned line_num)
{
    ast_node_t *current_node = ast_node_init(AST_RETURN, line_num);

    current_node->data->return_node.expr_or_call = expr_or_call;

    return current_node;
}



//  GLOBAL INSERT FUNCTIONS

ast_node_t *ast_insert_fn_definition(char *value, unsigned line_num, token_type data_type,
                                        ast_node_t *first_param, ast_node_t *body, bool is_main)
{
    ast_node_t *current_node = ast_node_init(AST_DEFINITION, line_num);

    current_node->data->definition_node.id_val = value;
    current_node->data->definition_node.data_type = data_type;
    current_node->data->definition_node.first_param = first_param;
    current_node->data->definition_node.body = body;
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


ast_node_t *expr_insert_id_leaf(char *value, unsigned line_num)
{
    ast_node_t *leaf_node = ast_node_init(EXPR_IDENTIFIER, line_num);
    leaf_node->data->id_leaf_node.id_val = value;

    return leaf_node;
}


ast_node_t *expr_insert_lit_leaf(token_value_t lit_value, unsigned line_num, token_type lit_type)
{
    ast_node_t *leaf_node = ast_node_init(EXPR_LITERAL, line_num);

    leaf_node->data->lit_leaf_node.lit_type = lit_type;
    leaf_node->data->lit_leaf_node.lit_value = lit_value;

    return leaf_node;
}
