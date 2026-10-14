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


/**
 * @brief Calls individual parsing functions for each case of next_token
 *
 * @param current Current token
 */
void parse_next(token_t *current);

/**
 * @brief Parses declaration of variables
 *
 * @param current Current token
 */
void parse_var_declaration(token_t *current);


#endif
