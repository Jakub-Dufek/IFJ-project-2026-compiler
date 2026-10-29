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
    if(current_token.type != T_IDENTIFIER) {
        error_exit(SYNT_ERR, current_token.line_num, "Incorrect prologue format");
    }
    else if(strcmp(current_token.value.string_value, "ifj26") != 0) {
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

    // newline or eof
    current_token = get_next_token();
    if(current_token.type != T_EOF && current_token.type != T_LF) {
        error_exit(SYNT_ERR, current_token.line_num, "Incorrect prologue format");
    }
}



//  HELPER PARSERS

/**
 * TODO: comment
 */
static ast_node_t *parse_block()
{
    //TODO: block parsing
}

/**
 * @brief Parses [: type] in declaration (call after recieving T_COLON)
 * 
 * @return data type
 */
static token_type parse_optional_type_def()
{
    token_t current_token = get_next_token();
    token_type data_type;

    // if next token anything but data type keyword
    if(current_token.type <= T_KEY_INT || current_token.type >= T_KEY_UNIT) {
        error_exit(SYNT_ERR, current_token.line_num, "Expected data type");
    }
    else {
        data_type = current_token.type;
    }
}


/**
 * @brief Parses parameters through recursion
 * 
 * @param current_token
 * 
 * @return pointer to first param, ptr->next is next param, NULL if no param
 */
static ast_node_t *parse_param(token_t current_token)
{
    // end of recursion
    if(current_token.type == T_R_ROUND_BRACKET) {
        return NULL;
    }

    // id
    if(current_token.type != T_IDENTIFIER) {
        error_exit(SYNT_ERR, current_token.line_num, "Incorrect function parameter format");
    }
    char value[MAX_LENGTH];
    strcpy(value, current_token.value.string_value);
    unsigned line_num = current_token.line_num;

    // [: type]
    current_token = get_next_token();
    token_type data_type = T_NO_TYPE;

    if(current_token.type == T_COLON) {
        data_type = parse_optional_type_def();
        current_token = get_next_token();
    }

    // ,
    if(current_token.type == T_COMMA) {
        current_token = get_next_token();
    }

    return ast_insert_param(value, line_num, data_type, parse_param(current_token)); // recursive calling
}



//  LOCAL PARSING FUNCTIONS

ast_node_t *parse_var_declaration(token_t current_token)
{
    // var/val
    bool is_mutable = false;
    if(current_token.type == T_KEY_VAR) {
        is_mutable = true;
    }

    // id
    current_token = get_next_token();
    if(current_token.type != T_IDENTIFIER) {
        error_exit(SYNT_ERR, current_token.line_num, "Expected identifier");
    }
    char value[MAX_LENGTH];
    strcpy(value, current_token.value.string_value);
    unsigned line_num = current_token.line_num;

    // [: type]
    current_token = get_next_token();
    token_type data_type = T_NO_TYPE;

    if(current_token.type == T_COLON) {
        data_type = parse_optional_type_def();
        current_token = get_next_token();
    }

    // =
    if(current_token.type != T_ASSIGN) {
        error_exit(SYNT_ERR, current_token.line_num, "Expected assign or colon");
    }

    // expression
    ast_node_t *expression = parse_expression();
    return ast_insert_var_declaration(value, line_num, is_mutable, data_type, expression);
}



//  GLOBAL PARSING FUNCTIONS

ast_node_t *parse_main_fn_definition()
{
    // after @main
    token_t current_token = get_next_token();

    while(current_token.type != T_KEY_DEF) {
        if(current_token.type == T_EOF) {
            error_exit(SYNT_ERR, current_token.line_num, "Incomplete @main function definition");
        }
        else if(current_token.type != T_LF) {
            error_exit(SYNT_ERR, current_token.line_num, "Unexpected token at global level");
        }
        current_token = get_next_token();
    }

    ast_node_t *fn = parse_fn_definition();
    fn->data->definition_node.is_main = true;
    return fn;
}


ast_node_t *parse_fn_definition()
{
    // fn id
    token_t current_token = get_next_token();
    if(current_token.type != T_IDENTIFIER) {
        error_exit(SYNT_ERR, current_token.line_num, "Incorrect function definition format");
    }
    char value[MAX_LENGTH];
    strcpy(value, current_token.value.string_value);
    unsigned line_num = current_token.line_num;

    // (
    current_token = get_next_token();
    if(current_token.type != T_L_ROUND_BRACKET) {
        error_exit(SYNT_ERR, current_token.line_num, "Incorrect function definition format");
    }

    // params )
    current_token = get_next_token();
    ast_node_t *param = parse_param(current_token);

    // [: type]
    current_token = get_next_token();
    token_type data_type = T_NO_TYPE;

    if(current_token.type == T_COLON) {
        data_type = parse_optional_type_def();
        current_token = get_next_token();
    }

    // =
    current_token = get_next_token();
    if(current_token.type != T_ASSIGN) {
        error_exit(SYNT_ERR, current_token.line_num, "Expected colon or assign");
    }
    
    // TODO: giant if elif else part with determining if stmt or call or block with newline etc


    //TODO: INDENT DEDENT

    return ast_insert_fn_definition(value, line_num, T_KEY_UNIT, NULL, NULL, true); //TODO: everything past T_KEY_UNIT
}



//  MAIN PARSING SWITCHES

ast_node_t *parse_next_local(token_t current_token)
{
    switch(current_token.type) {
        case T_KEY_VAR: // fall through below
        case T_KEY_VAL:
            return parse_var_declaration(current_token);
        // TODO: OTHER CASES
        default:
            error_exit(SYNT_ERR, current_token.line_num, "Unexpected token at local level");
    }
}


ast_node_t *parse_next_global(token_t current_token)
{
    bool is_main = false;
    switch(current_token.type) {
        case T_AT_MAIN:
            return parse_main_fn_definition();
        case T_KEY_DEF:
            return parse_fn_definition();
        case T_LF:
            break;
        case T_EOF:
            break;
        default:
            error_exit(SYNT_ERR, current_token.line_num, "Unexpected token at global level");
    }
}



//  MAIN PARSER LOOPS

ast_node_t *build_ast()
{
    parse_prologue();
    
    token_t current_token = get_next_token();
    ast_node_t *head = NULL;    // ptr pointing to start of ast
    ast_node_t *tail = NULL;    // ptr pointing to current node in ast building process (end)

    while(current_token.type != T_EOF) {
        // setting head at beginning
        if(head == NULL) {
            tail = parse_next_global(current_token);
            head = tail;
        }
        else {
            tail->next = parse_next_global(current_token);
            tail = tail->next;
        }

        current_token = get_next_token();
    }

    return head;
}
