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
#include "structures.h"
#include <ctype.h>
#include <stdio.h>

token_t handle_white_spaces(int* character) {
    token_t token = {0};

    character = character;  //Placeholder for compiler to shut up. Remove once you use character variable

    return token;
}

token_t first_alpha (int character) {
    token_t token = {0};
    int current_case = 0;

    do {
        switch (current_case) {
            case 0:
                break;
        }
    } while(!isspace(character = getchar()));

    ungetc(character, stdin);
    return token;
}

token_t first_digit (int character) {
    token_t token = {0};
    int current_case = 0;

    do {
        switch (current_case) {
            case 0:
                break;
        }
    } while(!isspace(character = getchar()) && character != EOF);

    ungetc(character, stdin);
    return token;
}

token_t first_special (int character) {
    token_t token = {0};
    int current_case = 0;

    do {
        switch (current_case) {
            case 0:
                break;
        }
    } while(!isspace(character = getchar()));

    ungetc(character, stdin);
    return token;
}

token_t scanner_fsm (/*int argc, char** argv*/) {
    //Initialize token and variable for storing read characters
    token_t token = {0};
    int character;

    //while (isspace(character = getchar())) {}

    //Handle white spaces and if some if them is a token, return token
    token = handle_white_spaces(&character);
    if (token.type != T_UNDEFINED) {
        return token;
    }

    if (isalpha(character)) {
        token = first_alpha(character);
    }
    else if (isdigit(character)) {
        token = first_digit(character);
    }
    else {
        token = first_special(character);
    }

    return token;
}