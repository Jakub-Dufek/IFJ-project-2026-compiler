/**
 * @file parser.c
 * @internal Part of IFJ-project-2026-compiler, Team 274855
 *
 * @brief Source file for recursive descent parsing
 * @author Jakub Dufek (274855)
 * @date 3. 10. 2026
 */

#include "parser.h"
#include "expr_parser.h"
#include "ast.h"

#include "structures.h"
#include "scanner.h"
#include "error.h"

#include <stdbool.h>


// INDIVIDUAL PARSING FUNCTIONS

void parse_var_declaration(token_t *current)
{
    bool is_mutable = false;
    if(current->type == T_KEY_VAR) {
        is_mutable = true;
    }

    *current = get_next_token();
    if(current->type != T_IDENTIFIER) {
        error_exit(SYNT_ERR, current->line_num, "Expected identifier");
    }

    char value[MAX_LENGTH] = current->value.string_value;
    unsigned line_num = current->line_num;

    *current = get_next_token();
    ast_data_type_t data_type;

    // TODO: put into separate helper function
    if(current->type == T_ASSIGN) {
        data_type = AST_NO_T;
    }
    else if(current->type == T_KEY_BOOLEAN) {
        data_type = AST_BOOL_T;
    }
    else if(current->type == T_KEY_DOUBLE) {
        data_type = AST_DOUBLE_T;
    }
    else if(current->type == T_KEY_INT) {
        data_type = AST_INT_T;
    }
    else if(current->type == T_KEY_STRING) {
        data_type = AST_STRING_T;
    }
    else if(current->type == T_KEY_UNIT) {
        data_type = AST_UNIT_T;
    }
    else {
        error_exit(SYNT_ERR, line_num, "Expected data type or assign");
    }

    ast_node_t *expression = parse_expression();
    ast_insert_var_declaration(value, is_mutable, data_type, expression);
}



//  MAIN PARSING SWITCH

void parse_next(token_t *current)
{
    // TODO: FIRST EXPRESSION PROGRAMME BEGIN

    switch(current->type) {
        case T_KEY_VAR || T_KEY_VAL:
            parse_var_declaration(current);
        // TODO: OTHER CASES
    }
}



//  TEMP MAIN

int main()
{
    token_t *current;
    *current = get_next_token();

    parse(current);
}
