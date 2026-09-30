/**
* @file scanner.c
* @internal Part of IFJ-project-2026-compiler
*
* @brief Library file containing structures
* @author Šimon Greger (xgreges00) 
* @date 27.09.2026
* 
* @details This file contains finite state machine representing the functions of a scanner
*/

//libraries
#include "scanner.h"
#include <ctype.h>
#include <stdio.h>
#include <stdbool.h>

//global variables
static bool line_start = true;
static unsigned depth_of_brackets = 0;
static unsigned indent_depth = 0;
static unsigned past_indent_depth = 0;

//functions
token_t handle_comment (int character, token_t token, scanner_case current_case) {
    switch (current_case) {
        case CASE_LINE_COMMENT:
            //throw away contents of line comment
            while ((character = getchar()) != '\n' && character != EOF) { }
            //create token indicating end of line
            if (character == '\n') {
                token.type = T_LF;
            }
            else {
                token.type = T_EOF;
            }
            break;
        case CASE_BLOCK_COMMENT:
            unsigned block_depth = 1;
            bool multiline_block = false;
            while (block_depth > 0 && (character = getchar()) != EOF) {
                switch (character) {
                    case '/':
                        if ((character = getchar()) == '*') {
                            block_depth++;
                        }
                        else if (character == '\n') {
                            token.line_num++;
                            multiline_block = true;
                        }
                        else if (character != EOF) {
                            ungetc(character, stdin);
                        }
                        break;
                    case '*':
                        if ((character = getchar()) == '/') {
                            block_depth--;
                        }
                        else if (character == '\n') {
                            token.line_num++;
                            multiline_block = true;
                        }
                        else if (character != EOF) {
                            ungetc(character, stdin);
                        }
                        break;
                    case '\n':
                        token.line_num++;
                        multiline_block = true;
                        break;
                    default:
                        break;
                }
            }

            if (block_depth > 0 && character == EOF) {
                //#TODO... CHYBA: konec souboru před tím, než byl ukončen blokový komentář
            }
            else if (multiline_block == true) {
                //Check if the rest of the line is empty
                while ((character = getchar()) != '\n' && character != EOF) {
                    if (!isspace(character)) {
                        //#TODO... CHYBA: po komentáři není prázdný řádek 
                    }
                }
                token.type = T_LF;
            }
            else if (multiline_block == false && line_start == true) {
                indent_depth++;
            }
            break;
        default:
            break;
    }

    return token;
}

token_t handle_white_spaces (int* character, token_t token, lexeme_t *lexeme, scanner_case current_case) {
    while (isspace(*character = getchar())) {
        indent_depth++;
        switch (current_case) {
            case CASE_START:
                
                break;
            default:
                break;
        }
    }

    if (*character == EOF) {
        token.type = T_EOF;
    }
    return token;
}

token_t first_alpha (int character, token_t token, lexeme_t *lexeme, scanner_case current_case) {
    do {
        //note the lexeme's value
        lexeme->value[lexeme->size] = (char)character;
        lexeme->size++;
        lexeme->value[lexeme->size] = '\0';

        switch (current_case) {
            case CASE_START:
                break;
            default:
                break;
        }
    } while(!isspace(character = getchar()) && character != EOF);   //#TODO... Better condition, this one does not handle everything (př. x+2)

    //Return last read character, that was not used
    if (character != EOF) { ungetc(character, stdin); }
    return token;
}

token_t first_digit (int character, token_t token, lexeme_t *lexeme, scanner_case current_case) {
    do {
        //note the lexeme's value
        lexeme->value[lexeme->size] = (char)character;
        lexeme->size++;
        lexeme->value[lexeme->size] = '\0';

        switch (current_case) {
            case CASE_START:
                break;
            default:
                break;
        }
    } while(!isspace(character = getchar()) && character != EOF);   //#TODO... Better condition, this one does not handle everything (př. x+2)

    //Return last read character, that was not used
    if (character != EOF) { ungetc(character, stdin); }
    return token;
}

token_t first_special (int character, token_t token, lexeme_t *lexeme, scanner_case current_case) {
    do {
        //note the lexeme's value
        lexeme->value[lexeme->size] = (char)character;
        lexeme->size++;
        lexeme->value[lexeme->size] = '\0';

        switch (current_case) {
            case CASE_START:
                switch (character){
                    case '/':
                        current_case = CASE_SLASH;
                        break;
                    default:
                        break;
                }
                break;

            case CASE_SLASH:
                switch (character) {
                    case '/':
                        current_case = CASE_LINE_COMMENT;
                        token = handle_comment(character, token, current_case);
                        return token;
                        break;
                    case '*':
                        current_case = CASE_BLOCK_COMMENT;
                        token = handle_comment(character, token, current_case);
                        return token;
                        break;
                    default:
                        token.type = T_DIV;
                        break;
                }
            default:
                break;
        }
    } while(!isspace(character = getchar()) && character != EOF);   //#TODO... Better condition, this one does not handle everything (př. x+2)

    //Return last read character, that was not used
    if (character != EOF) { ungetc(character, stdin); }
    return token;
}

token_t scanner_fsm (unsigned line_num) {
    //Initialize token and variable for storing read characters
    token_t token = {0};
    token.line_num = line_num;
    lexeme_t lexeme = {0};
    scanner_case current_case = CASE_START;
    int character;

    //
    while (token.type == T_UNDEFINED) {
        //Handle white spaces and if some if them is a token, leave loop
        token = handle_white_spaces(&character, token, &lexeme, current_case);
        if (token.type != T_UNDEFINED) {
            break;
        }

        if (isalpha(character)) {
            token = first_alpha(character, token, &lexeme, current_case);
        }
        else if (isdigit(character)) {
            token = first_digit(character, token, &lexeme, current_case);
        }
        else {
            token = first_special(character, token, &lexeme, current_case);
        }
    }

    return token;
}