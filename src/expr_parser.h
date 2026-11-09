/**
* @file expr_parser.h
* @internal Part of IFJ-project-2026-compiler, Team 274855
*
* @brief TODO
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

/* typedef struct token_array {
    token_t *tokens;
    int current;
    int max;
} TToken_array; */

precedence_table_index table_index(token_type type);

ast_node_t *parse_expression(token_t token);

void grammar_check(TStack *stack, TStack_Element *array, int array_counter);

#endif
