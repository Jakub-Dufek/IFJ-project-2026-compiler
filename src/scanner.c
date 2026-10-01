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
static token_t next_token;
static unsigned current_line_num = 1;

token_t new_token () {
    token_t new_token = {0};
    new_token.line_num = current_line_num;
    return new_token;
}

//functions
token_t handle_comment (int character, token_t token, scanner_case current_case) {
    switch (current_case) {
        case CASE_LINE_COMMENT:
            //throw away contents of line comment
            while ((character = getchar()) != '\n' && character != EOF) { }
            //create token indicating end of line
            if (character == '\n' && line_start == false) {
                token.type = T_LF;
                current_line_num++;
                token.line_num++;
            }
            else if (character == EOF) {
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
                            current_line_num++;
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
                            current_line_num++;
                            multiline_block = true;
                        }
                        else if (character != EOF) {
                            ungetc(character, stdin);
                        }
                        break;
                    case '\n':
                        token.line_num++;
                        current_line_num++;
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
                current_line_num++;
                token.line_num++;
            }
            break;
        default:
            break;
    }

    return token;
}

token_t handle_white_spaces (int* character, token_t token, lexeme_t *lexeme, scanner_case current_case) {
    while (token.type == T_UNDEFINED && isspace(*character = getchar())) {
        if (line_start == true) {
            indent_depth++;
        }
        switch (*character) {
            case '\t':
                if (line_start == true) {
                    //#TODO... Chyba: tabulátor na začátku
                }
                break;
            case '\n':
                //do not send T_LF token if one of these conditions happen
                if (line_start == false && depth_of_brackets == 0) {
                    token.type = T_LF;
                }
                token.line_num++;
                current_line_num++;
                indent_depth = 0;
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
                        lexeme->size = 0;
                        current_case = CASE_LINE_COMMENT;
                        token = handle_comment(character, token, current_case);
                        return token;
                        break;
                    case '*':
                        lexeme->size = 0;
                        current_case = CASE_BLOCK_COMMENT;
                        token = handle_comment(character, token, current_case);
                        return token;
                        break;
                    default:
                        token.type = T_DIV;
                        ungetc(character, stdin);
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

token_t scanner_fsm () {
    //Initialize token and variable for storing read characters
    token_t token = new_token();
    lexeme_t lexeme = {0};
    scanner_case current_case = CASE_START;
    int character;

    //Main loop for finding and creating token
    while (token.type == T_UNDEFINED && next_token.type == T_UNDEFINED) {
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

    if (next_token.type != T_UNDEFINED) {
        token = next_token;
        next_token = new_token(0);
    }

    //Handle indent
    if (line_start == true && token.type != T_LF) {
        line_start = false;
        if (depth_of_brackets == 0 && indent_depth == past_indent_depth) {
            next_token = token;
            if (indent_depth > past_indent_depth) {
                token.type = T_MORE_INDENT;
            }
            else if (indent_depth < past_indent_depth) {
                token.type = T_LESS_INDENT;
            }
            past_indent_depth = indent_depth;
        }
        else if (depth_of_brackets > 0 && indent_depth <= past_indent_depth) {
            //#TODO... chyba: v závorkách je menší úroveň odsazení než před nimi
        }
    }
    else if (line_start == false && token.type == T_LF) {
        line_start = true;
    }

    indent_depth = 0;
    return token;
}