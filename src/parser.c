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



//  PROLOGUE PARSING

void parse_prologue()
{
    // import
    token_t current_token = get_next_token();
    if(current_token.type != T_KEY_IMPORT) {
        error_exit(SYNT_ERR, 0, "Programme missing prologue");
    }

    // ifj26
    current_token = get_next_token();
    if(current_token.type != T_UNDEFINED) { //TODO: switch for ifj26 keyword
        error_exit(SYNT_ERR, current_token.line_num, "Incorrect prologue format");
    }

    // .
    current_token = get_next_token();
    if(current_token.type != T_DOT) {
        error_exit(SYNT_ERR, current_token.line_num, "Incorrect prologue format");
    }

    // *
    current_token = get_next_token();
    if(current_token.type != T_MULT) {
        error_exit(SYNT_ERR, current_token.line_num, "Incorrect prologue format");
    }

    //TODO: end of line
}



//  GLOBAL OPARSING FUNCTIONS

ast_node_t *parse_fn_definition()
{
    // TODO: implement
}



//  INSIDE-BLOCK PARSING FUNCTIONS

ast_node_t *parse_var_declaration(token_t current_token)
{
    // collect data from var/val token  
    bool is_mutable = false;
    if(current_token.type == T_KEY_VAR) {
        is_mutable = true;
    }

    current_token = get_next_token();
    if(current_token.type != T_IDENTIFIER) {
        error_exit(SYNT_ERR, current_token.line_num, "Expected identifier");
    }

    // collect data from id token
    char value[MAX_LENGTH];
    strcpy(value, current_token.value.string_value);
    unsigned line_num = current_token.line_num;

    // determine whether to define data type or not
    current_token = get_next_token();
    token_type data_type;

    // no type specified
    if(current_token.type == T_ASSIGN) {
        data_type = T_NO_TYPE;
    }
    else if(current_token.type == T_COLON) {
        current_token = get_next_token();
        // if next token anything but data type keyword
        if(current_token.type <= T_KEY_INT || current_token.type >= T_KEY_UNIT) {
            error_exit(SYNT_ERR, current_token.line_num, "Expected data type");
        }
        else {
            data_type = current_token.type;
        }
    }
    else {
        error_exit(SYNT_ERR, current_token.line_num, "Expected assign or colon");
    }

    ast_node_t *expression = parse_expression();
    return ast_insert_var_declaration(value, line_num, is_mutable, data_type, expression);
}



//  MAIN PARSING SWITCH

// TODO: split into 2 functions
ast_node_t *parse_next(token_t current_token)
{
    switch(current_token.type) {
        case T_KEY_VAR: // fall through below
        case T_KEY_VAL:
            return parse_var_declaration(current_token);
        case T_KEY_DEF:
            return parse_fn_definition();
        // TODO: OTHER CASES
        default:
            // TODO: some kind of error
    }
}



//  MAIN PARSER LOOP

ast_node_t *build_ast()
{
    parse_prologue();
    
    token_t current_token = get_next_token();
    ast_node_t *head = NULL;    // ptr pointing to start of ast
    ast_node_t *tail = NULL;    // ptr pointing to current node in ast building process (end)

    while(current_token.type != T_EOF) {
        // setting head at beginning
        if(head == NULL) {
            tail = parse_next(current_token);
            head = tail;
        }
        else {
            tail->next = parse_next(current_token);
            tail = tail->next;
        }

        current_token = get_next_token();
    }

    return head;
}
