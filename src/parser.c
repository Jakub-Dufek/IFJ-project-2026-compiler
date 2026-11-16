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
    // newline handling
    token_t current_token = get_next_token();
    while(current_token.type == T_LF) {
        current_token = get_next_token();
    }

    // import
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
 * @brief Determines whether upcoming tokens are expr or fn-call as part of another type of stmt
 * 
 * @param id_token identifier as first token after T_ASSIGN
 * 
 * @return subtree of expr or call as part of another type of stmt
 */
static ast_node_t *parse_expr_or_call(token_t id_token)
{
    token_t current_token = get_next_token();

    if(current_token.type == T_L_ROUND_BRACKET) {
        return_buffer_token(current_token);
        return parse_call(id_token);
    }
    else {
        return_buffer_token(current_token);
        return parse_expression(id_token);
    }
}


/**
 * @brief Determines whether upcoming tokens are standalone expr, standalone fn-call or assign
 * 
 * @param id_token identifier as first token in whole stmt
 * 
 * @return subtree of standalone expr, standalone call or assign
 */
static ast_node_t *parse_expr_call_or_assign(token_t id_token)
{
    token_t current_token = get_next_token();
    switch(current_token.type) {
        case T_ASSIGN:
            return_buffer_token(current_token);
            return parse_assign(id_token);
        case T_L_ROUND_BRACKET:
            return_buffer_token(current_token);
            return parse_call(id_token);
        default:
            return_buffer_token(current_token);
            return parse_expression(id_token);
    }
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
        // newline or semicolon
        if(current_token.type == T_LF || current_token.type == T_SEMICOLON) {
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
 * @brief Parses fn def parameters through recursion
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
    char *value = current_token.value.string_value;
    unsigned line_num = current_token.line_num;

    // :
    current_token = get_next_token();
    if(current_token.type != T_COLON) {
        error_exit(SYNT_ERR, current_token.line_num, "Expected data type for parameter");
    }

    // type
    token_type data_type = parse_data_type_def();
    current_token = get_next_token();

    // AST inserting
    return ast_insert_param(value, line_num, data_type, parse_param(current_token, false)); // recursive calling
}


/**
 * @brief Parses fn call arguments through recursion
 * 
 * @param current_token
 * @param is_first
 * 
 * @return pointer to first arg, ptr->next is next arg, NULL if no arg
 */
static ast_node_t *parse_arg(token_t current_token, bool is_first)
{
    // end of recursion
    if(current_token.type == T_R_ROUND_BRACKET) {
        return NULL;
    }

    // ,
    if(!is_first) {
        if(current_token.type != T_COMMA) {
            error_exit(SYNT_ERR, current_token.line_num, "Incorrect function call argument format");
        }
        unsigned comma_line_num = current_token.line_num;
        current_token = get_next_token();

        // edge case T_LF after comma handling
        if(current_token.type == T_R_ROUND_BRACKET) {
            if(comma_line_num < current_token.line_num) {
                return NULL;
            }
            else {
                error_exit(SYNT_ERR, current_token.line_num, "Incorrect function call argument format");
            }
        }
    }

    // unary minus before literal
    if(current_token.type == T_MINUS) {
        current_token = get_next_token();
        if(current_token.type == T_INT_LITERAL) {
            current_token.value.int_value = -current_token.value.int_value;
        }
        else if(current_token.type == T_DOUBLE_LITERAL) {
            current_token.value.double_value = -current_token.value.double_value;
        }
        else {
            error_exit(SYNT_ERR, current_token.line_num, "Incorrect function call argument format");
        }
    }

    // id or lit
    ast_node_t *arg;
    unsigned line_num = current_token.line_num;

    switch(current_token.type) {
        case T_IDENTIFIER:
            arg = expr_insert_id_leaf(current_token.value.string_value, line_num);
            break;
        case T_INT_LITERAL: case T_DOUBLE_LITERAL: case T_STRING_LITERAL: case T_UNIT_LITERAL:
            arg = expr_insert_lit_leaf(current_token.value, line_num, current_token.type);
            break;
        default:
            error_exit(SYNT_ERR, current_token.line_num, "Incorrect function call argument format");
    }

    arg->next = parse_arg(get_next_token(), false); // recursive calling
    return arg;
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
    char *value = current_token.value.string_value;
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

    // expression or call
    current_token = get_next_token();
    ast_node_t *expr_or_call;

    if(current_token.type == T_IDENTIFIER) {
        expr_or_call = parse_expr_or_call(current_token);
    }
    else {
        expr_or_call = parse_expression(current_token);
    }
    
    // AST insertion
    return ast_insert_var_declaration(value, line_num, is_mutable, data_type, expr_or_call);
}


ast_node_t *parse_assign(token_t current_token)
{
    // id
    char *value = current_token.value.string_value;
    unsigned line_num = current_token.line_num;

    // =
    current_token = get_next_token();
    if(current_token.type != T_ASSIGN) {
        error_exit(SYNT_ERR, current_token.line_num, "Expected assign or colon");
    }

    // expression or call
    current_token = get_next_token();
    ast_node_t *expr_or_call;

    if(current_token.type == T_IDENTIFIER) {
        expr_or_call = parse_expr_or_call(current_token);
    }
    else {
        expr_or_call = parse_expression(current_token);
    }
    
    // AST insertion
    return ast_insert_assign(value, line_num, expr_or_call);
}


ast_node_t *parse_call(token_t current_token)
{
    // id
    char *value = current_token.value.string_value;
    unsigned line_num = current_token.line_num;

    // (
    current_token = get_next_token();
    if(current_token.type != T_L_ROUND_BRACKET) {
        error_exit(SYNT_ERR, current_token.line_num, "Expected '(' after function call");
    }

    // arguments )
    current_token = get_next_token();
    ast_node_t *first_arg = parse_arg(current_token, true); // recursive series arg parsing

    // newline, eof, semicolon
    current_token = get_next_token();
    if(current_token.type != T_LF &&
       current_token.type != T_EOF &&
       current_token.type != T_SEMICOLON) {
        error_exit(SYNT_ERR, current_token.line_num, "Incorrect function call format");
    }

    return ast_insert_call(value, line_num, first_arg);
}


ast_node_t *parse_if()
{
    // condition
    token_t current_token = get_next_token();
    unsigned line_num = current_token.line_num;
    ast_node_t *condition = parse_expression(current_token);

    // then
    current_token = get_next_token();
    if(current_token.type != T_KEY_THEN) {
        error_exit(SYNT_ERR, current_token.line_num, "Expected 'then' after 'if' condition");
    }

    // newline
    current_token = get_next_token();
    if(current_token.type != T_LF) {
        error_exit(SYNT_ERR, current_token.line_num, "Expected newline after 'if then'");
    }

    // body
    ast_node_t *then_body = parse_block();

    // else
    current_token = get_next_token();
    if(current_token.type != T_KEY_ELSE) {
        error_exit(SYNT_ERR, current_token.line_num, "Expected 'else' after 'if then' block");
    }

    // newline
    current_token = get_next_token();
    if(current_token.type != T_LF) {
        error_exit(SYNT_ERR, current_token.line_num, "Expected newline after 'else'");
    }

    // block
    ast_node_t *else_body = parse_block();

    return ast_insert_if(condition, line_num, then_body, else_body);
}


ast_node_t *parse_while()
{
    // condition
    token_t current_token = get_next_token();
    unsigned line_num = current_token.line_num;
    ast_node_t *condition = parse_expression(current_token);
    // do
    current_token = get_next_token();
    if(current_token.type != T_KEY_DO) {
        error_exit(SYNT_ERR, current_token.line_num, "Expected 'do' after 'while' condition");
    }

    // newline
    current_token = get_next_token();
    if(current_token.type != T_LF) {
        error_exit(SYNT_ERR, current_token.line_num, "Expected newline after 'while do'");
    }

    // body
    ast_node_t *body = parse_block();

    return ast_insert_while(condition, line_num, body);
}


ast_node_t *parse_return()
{
    token_t current_token = get_next_token();

    if(current_token.type == T_LF ||
       current_token.type == T_EOF ||
       current_token.type == T_SEMICOLON) {
        return ast_insert_return(NULL, current_token.line_num);
    }
    else if(current_token.type == T_IDENTIFIER) {
        return ast_insert_return(parse_expr_or_call(current_token), current_token.line_num);
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
    char *value = current_token.value.string_value;
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
    else if(current_token.type == T_IDENTIFIER) {
        body = parse_expr_or_call(current_token);
    }
    else {
        body = parse_expression(current_token);
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
            return parse_if();
        case T_KEY_WHILE:
            return parse_while();
        case T_KEY_RETURN:
            return parse_return();
        case T_IDENTIFIER:
            return parse_expr_call_or_assign(current_token);
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
