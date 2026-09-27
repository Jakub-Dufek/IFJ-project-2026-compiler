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

#include "scanner.h"
#include <ctype.h>
#include <stdio.h>

token_t handle_white_spaces(int* character) {
    token_t token = {0};

    //#TODO... Loop for checking and clearing white spaces. Has to inicialise character variable

    character = character;  //Placeholder for compiler to shut up. Remove once you use character variable

    return token;
}

token_t first_alpha (int character, token_t token, lexeme_t *lexeme, int current_case) {
    do {
        lexeme->value[lexeme->size] = (char)character;
        lexeme->size++;
        lexeme->value[lexeme->size] = '\0';

        switch (current_case) {
            case 0:
                break;
        }
    } while(!isspace(character = getchar()) && character != EOF);   //#TODO... Better condition, this one does not handle everything (př. x+2)

    //Return last read character, that was not used
    if (character != EOF) { ungetc(character, stdin); }
    return token;
}

token_t first_digit (int character, token_t token, lexeme_t *lexeme, int current_case) {
    do {
        lexeme->value[lexeme->size] = (char)character;
        lexeme->size++;
        lexeme->value[lexeme->size] = '\0';

        switch (current_case) {
            case 0:
                break;
        }
    } while(!isspace(character = getchar()) && character != EOF);   //#TODO... Better condition, this one does not handle everything (př. x+2)

    //Return last read character, that was not used
    if (character != EOF) { ungetc(character, stdin); }
    return token;
}

token_t first_special (int character, token_t token, lexeme_t *lexeme, int current_case) {
    do {
        lexeme->value[lexeme->size] = (char)character;
        lexeme->size++;
        lexeme->value[lexeme->size] = '\0';

        switch (current_case) {
            case 0:
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
    int current_case = 0;
    int character;

    //while (isspace(character = getchar())) {}

    //Handle white spaces and if some if them is a token, return token
    token = handle_white_spaces(&character);
    if (token.type != T_UNDEFINED) {
        return token;
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

    return token;
}