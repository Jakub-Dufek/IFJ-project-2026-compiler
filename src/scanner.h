/**
* @file scanner.h
* @internal Part of IFJ-project-2026-compiler
*
* @brief Library file containing structures
* @author Šimon Greger (xgreges00)
* @date 27.09.2026
* 
* @details This file contains definitions of functions used in scanner.c
*/

#ifndef SCANNER
#define SCANNER

#include "structures.h"


typedef enum {
    CASE_START,

    CASE_SLASH,
    CASE_LINE_COMMENT,
    CASE_BLOCK_COMMENT,
<<<<<<< HEAD
} scanner_case;

/**
=======


} scanner_case;

/**
 * @brief Creates new token and initialises it
 * 
 * @param line_num Number holding on which like the token is located
 * 
 * @return New initialised token.
 */
token_t new_token (unsigned line_num);

/**
>>>>>>> scanner
 * @brief Removes comments from code
 * 
 * @param character Pointer to a variable holding one character from stdin.
 * @param token Structure that will hold the result of the function
 * @param current_case Variable representing the current case for switch
 * 
 * @return token_t structure either empty or with found T_LF token, ready to send
 */
token_t handle_comment (int character, token_t token, scanner_case current_case);

/**
 * @brief Handles white spaces
 * 
 * @param character Pointer to a variable holding one character from stdin.
 * @param token Structure that will hold the result of the function
 * @param lexeme Pointer to a structure used to store the value of the lexeme while reading it
 * @param current_case Variable representing the current case for switch
 * 
 * @return token_t structure either empty or with found token, ready to send
 */
<<<<<<< HEAD
token_t handle_white_spaces(int* character, token_t token, lexeme_t *lexeme, scanner_case current_case);
=======
token_t handle_white_spaces(int* character, token_t token);
>>>>>>> scanner

/**
 * @brief Handles all cases of tokens starting with an alphabetical character
 * 
 * @param character Variable holding one character from stdin.
 * @param token Structure that will hold the result of the function
 * @param lexeme Pointer to a structure used to store the value of the lexeme while reading it
 * @param current_case Variable representing the current case for switch
 * 
 * @return token_t structure, ready to send
 */
token_t first_alpha (int character, token_t token, lexeme_t *lexeme, scanner_case current_case);

/**
 * @brief Handles all cases of tokens starting with a digit character
 * 
 * @param character Variable holding one character from stdin.
 * @param token Structure that will hold the result of the function
 * @param lexeme Pointer to a structure used to store the value of the lexeme while reading it
 * @param current_case Variable representing the current case for switch
 * 
 * @return token_t structure, ready to send
 */
token_t first_digit (int character, token_t token, lexeme_t *lexeme, scanner_case current_case);

/**
 * @brief Handles all cases of tokens starting with all other possible characters
 * 
 * @param character Variable holding one character from stdin.
 * @param token Structure that will hold the result of the function
 * @param lexeme Pointer to a structure used to store the value of the lexeme while reading it
 * @param current_case Variable representing the current case for switch
 * 
 * @return token_t structure, ready to send
 */
token_t first_special (int character, token_t token, lexeme_t *lexeme, scanner_case current_case);

/**
 * @brief Scanner entry function. Declares all needed variables and starts the process of the fsm
 * 
 * @return token_t structure, ready to send
 */
token_t get_next_token ();

#endif