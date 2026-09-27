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


/**
 * @brief Handles white spaces
 * 
 * @param character Pointer to a variable holding one character from stdin.
 * 
 * @return token_t structure either empty or with found token, ready to send
 */
token_t handle_white_spaces(int* character);

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
token_t first_alpha (int character, token_t token, lexeme_t *lexeme, int current_case);


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
token_t first_digit (int character, token_t token, lexeme_t *lexeme, int current_case);


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
token_t first_special (int character, token_t token, lexeme_t *lexeme, int current_case);


/**
 * @brief Declares all needed variables and starts the process of the fsm
 * 
 * @return token_t structure, ready to send
 */
token_t scanner_fsm (unsigned line_num);

#endif