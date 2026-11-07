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
        error_exit(SYNT_ERR, current_token.line_num, "Incorrect prologue format (Expected: \"ifj26\")");
    }
    else if(strcmp(current_token.value.string_value, "ifj26") != 0) {
        error_exit(SYNT_ERR, current_token.line_num, "Incorrect prologue format (Expected: \"ifj26\")");
    }

    // .
    current_token = get_next_token();
    if(current_token.type != T_DOT) {
        error_exit(SYNT_ERR, current_token.line_num, "Incorrect prologue format (Expected: \".\")");
    }

    // *
    current_token = get_next_token();
    if(current_token.type != T_MULT) {
        error_exit(SYNT_ERR, current_token.line_num, "Incorrect prologue format (Expected: \"*\")");
    }

    // newline or eof
    current_token = get_next_token();
    if(current_token.type != T_EOF && current_token.type != T_LF) {
        error_exit(SYNT_ERR, current_token.line_num, "Incorrect prologue format");
    }
}



//  HELPER PARSERS

/**
 * @brief Parses [: type] in declaration (call after recieving T_COLON)
 * 
 * @return data type
 */
static token_type parse_data_type_def()
{
    token_t current_token = get_next_token();
    token_type data_type;

    // if next token anything but data type keyword
    if(current_token.type < T_KEY_INT || current_token.type > T_KEY_UNIT) {
        error_exit(SYNT_ERR, current_token.line_num, "Expected data type");
    }
    else {
        data_type = current_token.type;
    }

    return data_type;
}


/**
 * @brief Determines whether upcoming tokens are expr or fn-call
 * 
 * @return subtree of expr or call
 */
static ast_node_t *parse_expr_or_call()
{
    //TODO: parse_expr_or_call

    return NULL;
}



//  DISCRETE PARSERS

/**
 * @brief Parses block under function, if or while; call after recieving newline
 * 
 * @return subtree of upcoming block with ptr->first being first stmt in block
 */
static ast_node_t *parse_block()
{
    // indent
    token_t current_token = get_next_token();
    unsigned line_num = current_token.line_num;
    if(current_token.type != T_MORE_INDENT) {
        error_exit(SYNT_ERR, current_token.line_num, "Missing indent at beginning of block");
    }

    // parse local stmts
    current_token = get_next_token();
    ast_node_t *first = parse_next_local(current_token);
    ast_node_t *last = first;

    current_token = get_next_token();
    while(current_token.type != T_LESS_INDENT && current_token.type != T_EOF) {
        // newline
        if(current_token.type == T_LF) {
            current_token = get_next_token();
            continue;
        }
        last->next = parse_next_local(current_token);
        last = last->next;
        current_token = get_next_token();
    }

    return ast_insert_block(first, line_num, last);
}


/**
 * @brief Parses parameters through recursion
 * 
 * @param current_token
 * @param is_first
 * 
 * @return pointer to first param, ptr->next is next param, NULL if no param
 */
static ast_node_t *parse_param(token_t current_token, bool is_first)
{
    // end of recursion
    if(current_token.type == T_R_ROUND_BRACKET) {
        return NULL;
    }

    // ,
    if(!is_first) {
        if(current_token.type != T_COMMA) {
            error_exit(SYNT_ERR, current_token.line_num, "Incorrect function parameter format");
        }
        unsigned comma_line_num = current_token.line_num;
        current_token = get_next_token();

        // edge case T_LF after comma handling
        if(current_token.type == T_R_ROUND_BRACKET) {
            if(comma_line_num < current_token.line_num) {
                return NULL;
            }
            else {
                error_exit(SYNT_ERR, current_token.line_num, "Incorrect function parameter format");
            }
        }
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

    if(current_token.type != T_COLON) {
        error_exit(SYNT_ERR, current_token.line_num, "Expected data type for parameter");
    }
    data_type = parse_data_type_def();
    current_token = get_next_token();

    return ast_insert_param(value, line_num, data_type, parse_param(current_token, false)); // recursive calling
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
        data_type = parse_data_type_def();
        current_token = get_next_token();
    }

    // =
    if(current_token.type != T_ASSIGN) {
        error_exit(SYNT_ERR, current_token.line_num, "Expected assign or colon");
    }

    // expression
    ast_node_t *expression = parse_expression(get_next_token());
    return ast_insert_var_declaration(value, line_num, is_mutable, data_type, expression);
}


ast_node_t *parse_return()
{
    token_t current_token = get_next_token();

    if(current_token.type == T_LF || current_token.type == T_EOF) {
        return ast_insert_return(NULL, current_token.line_num);
    }
    else {
        return ast_insert_return(parse_expression(current_token), current_token.line_num);
    }
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
    ast_node_t *param = parse_param(current_token, true);

    // [: type]
    current_token = get_next_token();
    token_type data_type = T_NO_TYPE;

    if(current_token.type == T_COLON) {
        data_type = parse_data_type_def();
        current_token = get_next_token();
    }

    // =
    if(current_token.type != T_ASSIGN) {
        error_exit(SYNT_ERR, current_token.line_num, "Expected colon or assign");
    }

    // expr, call or block
    current_token = get_next_token();
    ast_node_t *body;
    if(current_token.type == T_LF) {
        body = parse_block();
    }
    else {
        body = parse_expr_or_call();
    }

    return ast_insert_fn_definition(value, line_num, data_type, param, body, false);
}



//  MAIN PARSING SWITCHES

ast_node_t *parse_next_local(token_t current_token)
{
    switch(current_token.type) {
        case T_KEY_VAR: case T_KEY_VAL:
            return parse_var_declaration(current_token);
        case T_KEY_IF:
            return NULL; // TODO: specify
        case T_KEY_WHILE:
            return NULL; // TODO: specify
        case T_KEY_RETURN:
            return NULL; // TODO: specify
        case T_IDENTIFIER:
            return NULL; // TODO: specify
        default:
            return parse_expression(current_token);
    }
}


ast_node_t *parse_next_global(token_t current_token)
{
    switch(current_token.type) {
        case T_AT_MAIN:
            return parse_main_fn_definition();
        case T_KEY_DEF:
            return parse_fn_definition();
        default:
            error_exit(SYNT_ERR, current_token.line_num, "Unexpected token at global level");
    }

    return NULL; // this will never be reached
}



//  MAIN PARSER LOOPS

ast_node_t *build_ast()
{
    parse_prologue();
    
    token_t current_token = get_next_token();
    ast_node_t *head = NULL;    // ptr pointing to start of ast
    ast_node_t *tail = NULL;    // ptr pointing to current node in ast building process (end)

    while(current_token.type != T_EOF) {
        // newline handling
        if(current_token.type == T_LF) {
            current_token = get_next_token();
            continue;
        }

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
