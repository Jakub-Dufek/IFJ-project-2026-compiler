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
#include <string.h>


// INDIVIDUAL PARSING FUNCTIONS

ast_node_t *parse_var_declaration(token_t *current)
{
    // collect data from var/val token  
    bool is_mutable = false;
    if(current->type == T_KEY_VAR) {
        is_mutable = true;
    }

    *current = get_next_token();
    if(current->type != T_IDENTIFIER) {
        error_exit(SYNT_ERR, current->line_num, "Expected identifier");
    }

    // collect data from id token
    char value[MAX_LENGTH];
    strcpy(value, current->value.string_value);
    unsigned line_num = current->line_num;

    // determine whether to define data type or not
    *current = get_next_token();
    token_type data_type;

    // no type specified
    if(current->type == T_ASSIGN) {
        data_type = T_NO_TYPE;
    }
    else if(current->type == T_COLON) {
        *current = get_next_token();
        // if next token anything but data type keyword
        if(current->type <= T_KEY_INT || current->type >= T_KEY_UNIT) {
            error_exit(SYNT_ERR, current->line_num, "Expected data type");
        }
        else {
            data_type = current->type;
        }
    }
    else {
        error_exit(SYNT_ERR, current->line_num, "Expected assign or colon");
    }

    ast_node_t *expression = parse_expression();
    return ast_insert_var_declaration(value, line_num, is_mutable, data_type, expression);
}



//  MAIN PARSING SWITCH

ast_node_t *parse_next(token_t *current)
{
    switch(current->type) {
        case T_KEY_VAR: // fall through below
        case T_KEY_VAL:
            return parse_var_declaration(current);
        // TODO: OTHER CASES
        default:
            // TODO: some kind of error
    }
}



//  TEMP MAIN

int main()
{
    ast_init_arena();

    token_t *current_token;
    ast_node_t *ast;

    do {
        *current_token = get_next_token();
        parse_next(current_token);
    } while(false);
}
