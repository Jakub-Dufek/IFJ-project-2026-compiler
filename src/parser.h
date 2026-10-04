/**
* @file parser.h
* @internal Part of IFJ-project-2026-compiler, Team 274855
*
* @brief TODO: comment
* @author Jakub Dufek (274855)
* @date 3. 10. 2026
*/

#ifndef PARSER_H
#define PARSER_H

/**
* @brief Calls individual parsing functions for each case of next_token
*
* @param current Current token
*/
void parse_next(token_t *current);

/**
* @brief parses declaration of variables
*
* @param current Current token
*/
void parse_var_declaration(token_t *current);

#endif
