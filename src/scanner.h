/**
 * @file scanner.h
 * @internal Part of IFJ-project-2026-compiler
 *
 * @brief Scanner library file containing structures
 * @author Šimon Greger (xgreges00)
 * @author Samuel Vít Lehotský (xlehots00)
 * @date 25.10.2026
 *
 * @details This file contains macros, structures and definitions of functions used in scanner.c
 */

#ifndef SCANNER_H
#define SCANNER_H

#define KEYWORDS_TABLE_NUM_ELEMENTS 24

#include "structures.h"
#include <stdbool.h>

typedef enum {
    CASE_START,

    CASE_SLASH,
    CASE_LINE_COMMENT,
    CASE_BLOCK_COMMENT,
    CASE_AT,
    CASE_QUOTATION_MARKS,

    CASE_INT_LITERAL,
    CASE_DOUBLE_LITERAL,
    CASE_EXPONENT,
} scanner_case;

/**
 * @struct keyword
 * @brief Structure representing a single keyword in the keywords dictionary
 *
 * This structure holds the string representation of a keyword and its token type
 */
typedef struct keyword {
    const char *string;
    token_type type;
} keyword;

/**
 * @brief Function for sending a token back to scanner
 *
 * @param token Token that is being returned to scanner
 *
 * The token that is being returned will be sent in the next call of scanner
 */
void return_buffer_token(token_t token);

/**
 * @brief Creates new token and initialises it
 *
 * @param line_num Number holding on which like the token is located
 *
 * @return New initialised token.
 */
token_t new_token(unsigned line_num);

/**
 * @brief Removes comments from code
 *
 * @param character Variable holding one character from stdin.
 * @param token Structure that will hold the result of the function
 * @param current_case Variable representing the current case for switch
 *
 * @return token_t structure either empty or with found T_LF token, ready to send
 */
token_t handle_comment(int character, token_t token, scanner_case current_case);

/**
 * @brief Removes comments from code
 *
 * @param character Variable holding one character from stdin.
 * @param multiline Variable indicating if the quotation is supposed to be multiline or not
 */
void handle_quotation(int character, bool multiline);

/**
 * @brief Handles white spaces
 *
 * @param character Pointer to a variable holding one character from stdin.
 * @param token Structure that will hold the result of the function
 *
 * @return token_t structure either empty or with found token, ready to send
 */
token_t handle_white_spaces(int *character, token_t token);

/**
 * @brief Handles levels of indentations on lines
 *
 * @param token Pointer to a structure holding output for this module
 */
void handle_indent(token_t *token);

/**
 * @brief Handles all cases of tokens starting with an alphabetical character
 *
 * @param character Variable holding one character from stdin.
 * @param token Structure that will hold the result of the function
 *
 * @return token_t structure, ready to send
 */
token_t first_alpha(int character, token_t token);

/**
 * @brief Handles all cases of tokens starting with a digit character
 *
 * @param character Variable holding one character from stdin.
 * @param token Structure that will hold the result of the function
 * @param current_case Variable representing the current case for switch
 *
 * @return token_t structure, ready to send
 */
token_t first_digit(int character, token_t token, scanner_case current_case);

/**
 * @brief Handles all cases of tokens starting with all other possible characters
 *
 * @param character Variable holding one character from stdin.
 * @param token Structure that will hold the result of the function
 * @param current_case Variable representing the current case for switch
 *
 * @return token_t structure, ready to send
 */
token_t first_special(int character, token_t token, scanner_case current_case);

/**
 * @brief Scanner entry function. Declares all needed variables and starts the process of the fsm
 *
 * @return token_t structure, ready to send
 */
token_t get_next_token();

#endif