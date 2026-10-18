/**
 * @file parser.h
 * @internal Part of IFJ-project-2026-compiler, Team 274855
 *
 * @brief Header file for recursive descent parsing
 * @author Jakub Dufek (274855)
 * @date 3. 10. 2026
 */

#ifndef PARSER_H
#define PARSER_H

#include "ast.h"


//  PROLOGUE PARSING

/**
 * @brief Parses prologue, is called at beginning of programme
 */
void parse_prologue();



//  LOCAL PARSING FUNCTIONS

/**
 * @brief Parses definition of function
 * 
 * @return pointer to top of syntax sub-tree
 */
ast_node_t *parse_fn_definition();



//  GLOBAL PARSING FUNCTIONS

/**
 * @brief Parses declaration of variables
 *
 * @param current Current token
 * 
 * @return pointer to top of syntax sub-tree
 */
ast_node_t *parse_var_declaration(token_t current);



//  GLOBAL PARSING SWITCHES

/**
 * @brief Calls individual parsing functions for each case of next_token within block
 *
 * @param current_token
 * 
 * @return pointer to top of syntax sub-tree
 */
ast_node_t *parse_next_local(token_t current_token);


/**
 * @brief Calls individual parsing functions for each case of next_token within block
 *
 * @param current_token
 * 
 * @return pointer to top of syntax sub-tree
 */
ast_node_t *parse_next_global(token_t current);



//  MAIN PARSER LOOP

/**
 * @brief Parses entire programme and builds AST for further analysis and compiling
 * 
 * @return pointer to top of AST
 */
ast_node_t *build_ast();


#endif
