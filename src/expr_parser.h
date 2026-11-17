/**
* @file expr_parser.h
* @internal Part of IFJ-project-2026-compiler, Team 274855
*
* @brief Header file for expression parsing
* @author Samuel Vít Lehotský (xlehots00)
* @date 3. 10. 2026
*/

#ifndef EXPR_PARSER_H
#define EXPR_PARSER_H

#include "ast.h"
#include "expr_parser_stack.h"

typedef enum {
    P_ADD_SUB,
    P_MUL_DIV,
    P_AND,
    P_OR,
    P_LT_LTE_GT_GTE,
    P_EQ_NEQ,
    P_L_ROUND_BRACKET,
    P_R_ROUND_BRACKET,
    P_UNARY_NEG_MINUS,
    P_ID,
    P_DOLLAR,

    PRECEDENCE_TABLE_SIZE
} precedence_table_index;

/**
 * @brief Returns index in the precedence table based on the token type
 *
 * @param type Type of the token
 * 
 * @return Index in the precedence table
 */
precedence_table_index table_index(token_type type);


/**
 * @brief Parses an expression using precedence analysis
 *
 * @param token First token of the expression
 * 
 * @return Pointer to the AST node containing the parsed expression
 */
ast_node_t *parse_expression(token_t token);


/**
 * @brief Checks reduction rules and builds corresponding AST node
 *
 * @param stack Pointer to the expression parsing stack
 * @param array Array containing stack elements for reduction
 * @param array_counter Number of elements in the array 
 */
void grammar_check(TStack *stack, TStack_Element *array, int array_counter);

#endif
