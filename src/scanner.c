/**
 * @file scanner.c
 * @internal Part of IFJ-project-2026-compiler
 *
 * @brief FSM for scanner
 * @author Šimon Greger (xgreges00)
 * @author Samuel Vít Lehotský (xlehots00)
 * @date 27.09.2026
 *
 * @details This file contains finite state machine representing the functions of a scanner
 */

// libraries
#include "scanner.h"
#include "error.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// global variables
static bool line_start = true;
static bool tab_on_line_start = false;
static unsigned depth_of_brackets = NO_VALUE;
static unsigned indent_depth = NO_VALUE;
static unsigned last_indent_idx = NO_VALUE;
static token_t next_token = {0};
static unsigned current_line_num = 1;
static unsigned indent_values[MAX_INDENT_LENGTH] = {NO_VALUE};

// dictionary
keyword keywords_dictionary[KEYWORDS_TABLE_NUM_ELEMENTS] = {
    {"Boolean", T_KEY_BOOLEAN}, {"Double", T_KEY_DOUBLE}, {"Int", T_KEY_INT},   {"List", T_KEY_LIST},
    {"String", T_KEY_STRING},   {"Unit", T_KEY_UNIT},     {"case", T_KEY_CASE}, {"class", T_KEY_CLASS},
    {"def", T_KEY_DEF},         {"do", T_KEY_DO},         {"else", T_KEY_ELSE}, {"extension", T_KEY_EXTENSION},
    {"false", T_KEY_FALSE},     {"for", T_KEY_FOR},       {"if", T_KEY_IF},     {"import", T_KEY_IMPORT},
    {"match", T_KEY_MATCH},     {"return", T_KEY_RETURN}, {"then", T_KEY_THEN}, {"true", T_KEY_TRUE},
    {"type", T_KEY_TYPE},       {"val", T_KEY_VAL},       {"var", T_KEY_VAR},   {"while", T_KEY_WHILE}};

token_t new_token(unsigned line_num)
{
    token_t new_token = {NO_VALUE};
    new_token.line_num = line_num;
    return new_token;
}

void write_lexeme(int character, lexeme_t *lexeme)
{
    // Check size of the lexeme
    if (lexeme->size + 1 >= MAX_LENGTH) {
        error_exit(INTERNAL_ERR, current_line_num, "Too long lexeme value, lexeme buffer overflow.");
    }
    // Write character value into the lexeme string
    lexeme->value[lexeme->size] = (char)character;
    lexeme->size++;
    lexeme->value[lexeme->size] = '\0';
    return;
}

// functions
token_t handle_comment(int character, token_t token, scanner_case current_case)
{
    switch (current_case) {
    case CASE_LINE_COMMENT:
        // throw away contents of line comment
        while ((character = getchar()) != '\n' && character != EOF) {
        }
        // handle end of line
        if (character == '\n') {
            if (line_start == false) { // if line is not empty, create token for end of line
                token.type = T_LF;
            } else {
                indent_depth = NO_VALUE;
                tab_on_line_start = false;
            }
            current_line_num++;
        } else if (character == EOF) {
            token.type = T_EOF;
        }
        break;
    case CASE_BLOCK_COMMENT: {
        unsigned block_depth = 1; // for handling nested comments
        bool multiline_block = false;
        while (block_depth > 0 && (character = getchar()) != EOF) {
            switch (character) {
            case '/':
                if ((character = getchar()) == '*') {
                    block_depth++;
                } else if (character == '\n') {
                    token.line_num++;
                    current_line_num++;
                    multiline_block = true;
                    tab_on_line_start = false;
                } else if (character != EOF) {
                    ungetc(character, stdin);
                }
                break;
            case '*':
                if ((character = getchar()) == '/') {
                    block_depth--;
                } else if (character == '\n') {
                    token.line_num++;
                    current_line_num++;
                    multiline_block = true;
                    tab_on_line_start = false;
                } else if (character != EOF) {
                    ungetc(character, stdin);
                }
                break;
            case '\n':
                tab_on_line_start = false;
                token.line_num++;
                current_line_num++;
                multiline_block = true;
                break;
            default:
                break;
            }
        }

        if (block_depth > 0 && character == EOF) {
            error_exit(LEX_ERR, token.line_num, "File ended inside of a block comment.");
        } else if (multiline_block == true) {
            // Check if the rest of the line is empty for multiline comment
            while ((character = getchar()) != '\n' && character != EOF) {
                if (!isspace(character)) {
                    error_exit(SYNT_ERR, token.line_num, "Unexpected character after multiline block comment.");
                }
            }
            if (character != EOF) {
                current_line_num++;
            }
            token.type = T_LF;
        }
        indent_depth = NO_VALUE;
        break;
    }
    default:
        break;
    }

    return token;
} // end of handle_comment()

void handle_quotation(int character, lexeme_t *lexeme, bool multiline)
{
    // no condition inside of while because there are if statements inside of this cycle that exit the cycle
    while (true) {
        switch (character) {
        case '\n':
            if (multiline == false) { // In case of one-line quotation, there can't be a symbol for end of the line
                error_exit(LEX_ERR, current_line_num, "One-line string does not end on the same line.");
            } else {
                write_lexeme(character, lexeme);
            }
            break;
        case '\\':
            // Handle escape sequences inside of one-line quotation
            if (multiline == false) {
                switch (character = getchar()) {
                case 'n':
                    write_lexeme('\n', lexeme);
                    break;
                case 't':
                    write_lexeme('\t', lexeme);
                    break;
                case 'r':
                    write_lexeme('\r', lexeme);
                    break;
                case '\\':
                    write_lexeme('\\', lexeme);
                    break;
                case '"':
                    write_lexeme('"', lexeme);
                    break;
                default:
                    error_exit(LEX_ERR, current_line_num, "Unsupported escape sequence used inside of a string literal.");
                    break;
                }
            } else {
                write_lexeme(character, lexeme);
            }
            break;
        case '"':
            // Handle end of while cycle conditions
            if (multiline == true && (character = getchar()) == '"') {
                if ((character = getchar()) == '"') {
                    return; // end multiline quotation
                } else {
                    write_lexeme('"', lexeme);
                    write_lexeme('"', lexeme);
                    write_lexeme(character, lexeme);
                }
            } else if (multiline == true) { // write everything read into lexeme if quotations is not ending
                write_lexeme('"', lexeme);
                write_lexeme(character, lexeme);
            } else if (multiline == false) { // end one-line quotation
                return;
            }
            break;
        case EOF:
            error_exit(LEX_ERR, current_line_num, "File ended inside of a string literal");
            break;
        default:
            write_lexeme(character, lexeme);
            break;
        }
        character = getchar(); // get next character
    }

    return;
} // end of handle_quotation

token_t handle_white_spaces(int *character, token_t token)
{
    while (token.type == T_UNDEFINED && isspace(*character = getchar())) {
        if (line_start == true && *character == ' ') {
            indent_depth++;
        }
        switch (*character) {
        case '\t':
            if (line_start == true) {
                tab_on_line_start = true;
            }
            break;
        case '\n':
            // do not send T_LF token if one of these conditions happen
            if (line_start == false && depth_of_brackets == 0) {
                token.type = T_LF;
            } else if (line_start == false && depth_of_brackets > 0) {
                line_start = true;
            } else if (line_start == true && tab_on_line_start == true) { // If the line is empty, using tab is not a misstake
                tab_on_line_start = false;
            }
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

void handle_indent(token_t *token)
{
    if (line_start == true && token->type != T_LF) { // When I find first lexeme,...
        if (tab_on_line_start == true) {             //...I check if tab was pressed somewhere before it...
            error_exit(LEX_ERR, token->line_num, "Tabulator found on the start of a non-empty line.");
        }

        line_start = false;
        if (depth_of_brackets == 0) {
            next_token = *token;
            *token = new_token(0);
            bool same_indent = false;
            //...and then I compare its indentation value with all values I have stored.
            for (unsigned idx = 0; idx <= last_indent_idx; idx++) {
                // If I find the indentation value in the list, but it is not the furthest,...
                if (indent_values[idx] == indent_depth && indent_values[last_indent_idx] != indent_depth) {
                    token->type = T_LESS_INDENT; //...I send token representing less indentation,...
                    token->line_num = current_line_num;
                    indent_values[last_indent_idx] = NO_VALUE; //...remove the furthest indentation,...
                    last_indent_idx--;                         //...subtract one from the indentation index...
                    line_start = true; //...and set line_start to true in case there is even less indentation
                }
                // If there is no change in indent, note it and delete next_token (no need for it)
                else if (indent_values[idx] == indent_depth && indent_values[last_indent_idx] == indent_depth) {
                    same_indent = true;
                    *token = next_token;
                    next_token = new_token(NO_VALUE);
                    break;
                }
            }
            // In case current indent is not in the list of previous indents...
            if (token->type != T_LESS_INDENT && same_indent == false) {
                // ...and the indent is lesser than the last indent, exit with error...
                if (indent_depth < indent_values[last_indent_idx]) {
                    error_exit(SYNT_ERR, token->line_num, "Invalid indentation level.");
                    // ...otherwise, note new indent value.
                } else {
                    if (last_indent_idx + 1 == MAX_INDENT_LENGTH) {
                        error_exit(INTERNAL_ERR, token->line_num, "Too many indentation levels, buffer overflow.");
                    } else {
                        last_indent_idx++;
                        indent_values[last_indent_idx] = indent_depth;
                        token->type = T_MORE_INDENT;
                        token->line_num = current_line_num;
                    }
                }
            }
        } else if (depth_of_brackets > 0 &&
                   indent_depth <= indent_values[last_indent_idx]) { // Check indent for inside of brackets
            error_exit(LEX_ERR, token->line_num, "Lesser or equal indentation level inside of brackets than before them.");
        }

        if (token->type == T_L_ROUND_BRACKET) {
            depth_of_brackets++;
        }
    } else if (token->type == T_LF) { // If the current line ends,...
        indent_depth = 0;             //...reset indent depth for another line...
        line_start = true;            //...and reset start of line variable
    }
    return;
} // end of return_indent()

int compare(const void *string, const void *keyword_string)
{
    const keyword *new_keyword = (const keyword *)keyword_string;
    return (strcmp((const char *)string, new_keyword->string));
}

token_t first_alpha (int character, token_t token, lexeme_t *lexeme) {
    bool valid = false;
    do {
        if (character != '_') { valid = true; }

        //note the lexeme's value
        write_lexeme(character, lexeme);

    } while ((isalnum(character = getchar()) || character == '_') && character != EOF);

    if (valid == false) { error_exit(LEX_ERR, current_line_num, "Invalid identifier."); }

    keyword *the_keyword = bsearch(lexeme->value, keywords_dictionary, KEYWORDS_TABLE_NUM_ELEMENTS, sizeof(keyword), compare);
    if (the_keyword == NULL) {
        token.type = T_IDENTIFIER;
    } else {
        token.type = the_keyword->type;
    }

     //Return last read character, that was not used
    if (character != EOF) { ungetc(character, stdin); }

    token.size = lexeme->size;
    strcpy(token.value.string_value, lexeme->value);

    return token;
}

token_t first_digit (int character, token_t token, lexeme_t *lexeme, scanner_case current_case) {
    int num_cnt_e, num_cnt_d = 0;
    bool valid = true;
    char last_character = character;
    do {
        if (character == '_') { last_character = character; continue; }
        
        switch (current_case) {
            case CASE_INT_LITERAL:
                if (character == '.') { current_case = CASE_DOUBLE_LITERAL; }
                else if (character == 'e' || character == 'E') { current_case = CASE_EXPONENT; }
                else if (character == 'd' || character == 'D') { current_case = CASE_DOUBLE_LITERAL; valid = false; }
                else if(!isdigit(character)) { valid = false; }
                break;
        
            case CASE_DOUBLE_LITERAL:
                if ((character == 'e' || character == 'E') && isdigit(last_character)) { current_case = CASE_EXPONENT; }
                else if (character == 'd' || character == 'D') { valid = false; }
                else if(!isdigit(character)) { valid = false; }
                if (isdigit(character)) {num_cnt_d++; }
                break;

            case CASE_EXPONENT:
                if ((character == '-' || character == '+') && last_character != 'e' && last_character != 'E') { 
                    valid = false; 
                }
                else if (!isdigit(character)) { valid = false; }
                if (isdigit(character)) { num_cnt_e++; }
                break;

            default:
                break;
            }

        if (valid) {
            //note the lexeme's value
            write_lexeme(character, lexeme);
        }
        else { break; }

        last_character = character;
    } while((isdigit(character = getchar()) || character == 'e' || character == 'E' || character == '-'
                                            || character == '.' || character == '_'
                                            || character == 'd' || character == 'D') && character != EOF);

    if ((current_case == CASE_EXPONENT && num_cnt_e == 0) || (current_case == CASE_DOUBLE_LITERAL && num_cnt_d == 0)) { 
        error_exit(LEX_ERR, current_line_num, "Missing value after decimal point/exponent.");
    }

    if (current_case == CASE_INT_LITERAL) { 
        token.type = T_INT_LITERAL;
        token.value.int_value = atoi(lexeme->value);
    }
    else if (current_case == CASE_DOUBLE_LITERAL || current_case == CASE_EXPONENT 
                                                 || character == 'd' || character == 'D') {
        token.type = T_DOUBLE_LITERAL;
        token.value.double_value = atof(lexeme->value);
    }

    // Return last read character, that was not used
    if (character != EOF && character != 'd' && character != 'D') {
        ungetc(character, stdin);
    }

    token.size = lexeme->size;

    return token;
}

token_t first_special(int character, token_t token, lexeme_t *lexeme, scanner_case current_case)
{
    do {
        // note the lexeme's value
        write_lexeme(character, lexeme);

        switch (current_case) {
        case CASE_START:
            switch (character) {
            case '+':
                token.type = T_PLUS;
                break;
            case '-':
                token.type = T_MINUS;
                break;
            case '*':
                token.type = T_MULT;
                break;
            case '/':
                current_case = CASE_SLASH;
                break;
            case '(':
                // If we are at the start of the line, depth of brackets will be increased in handle_indent()
                if (line_start != true) {
                    depth_of_brackets++;
                }
                token.type = T_L_ROUND_BRACKET;
                break;
            case ')':
                if (depth_of_brackets == 0) {
                    error_exit(SYNT_ERR, token.line_num, "Unmatched closing brackets.");
                }
                depth_of_brackets--;
                token.type = T_R_ROUND_BRACKET;
                break;
            case '<':
                if ((character = getchar()) == '=') {
                    token.type = T_LTE;
                } else {
                    token.type = T_LT;
                    if (character != EOF) {
                        ungetc(character, stdin);
                    }
                }
                break;
            case '>':
                if ((character = getchar()) == '=') {
                    token.type = T_GTE;
                } else {
                    token.type = T_GT;
                    if (character != EOF) {
                        ungetc(character, stdin);
                    }
                }
                break;
            case '=':
                if ((character = getchar()) == '=') {
                    token.type = T_EQ;
                } else {
                    token.type = T_ASSIGN;
                    if (character != EOF) {
                        ungetc(character, stdin);
                    }
                }
                break;
            case '!':
                if ((character = getchar()) == '=') {
                    token.type = T_NEQ;
                } else {
                    error_exit(LEX_ERR, token.line_num, "Invalid use of the '!' symbol.");
                }
                break;
            case ',':
                token.type = T_COMMA;
                break;
            case ':':
                token.type = T_COLON;
                break;
            case ';':
                token.type = T_SEMICOLON;
                break;
            case '.':
                token.type = T_DOT;
                break;
            case '@':
                current_case = CASE_AT;
                break;
            case '"':
                current_case = CASE_QUOTATION_MARKS;
                break;
            default:
                error_exit(LEX_ERR, token.line_num, "Unknown character.");
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
                if (character != EOF) {
                    ungetc(character, stdin);
                }
                break;
            }
            break;

        case CASE_AT:
            // Only @main is correct construction for a token starting with '@'
            if (character == 'm') {
                if ((character = getchar()) == 'a') {
                    if ((character = getchar()) == 'i') {
                        if ((character = getchar()) == 'n') {
                            /*  If statement created by ChatGPT.
                                Prompt: "Co má po @main správně pokračovat?"  */
                            if (isalnum((character = getchar())) || character == '_') {
                                error_exit(LEX_ERR, token.line_num, "Invalid @main anotation.");
                            } else if (character != EOF) {
                                ungetc(character, stdin);
                            }
                            token.type = T_AT_MAIN;
                        }
                    }
                }
            }
            if (token.type != T_AT_MAIN) {
                error_exit(LEX_ERR, token.line_num, "Invalid @main anotation.");
            }
            break;

        case CASE_QUOTATION_MARKS:
            // Remove quotation marks from the literal's value
            lexeme->size = NO_VALUE;
            lexeme->value[lexeme->size] = '\0';

            if (character == '"') {
                if ((character = getchar()) == '"') {
                    // Handles multiline quotation
                    handle_quotation(character = getchar(), lexeme, true);
                } else { // Token contains empty string literal
                    lexeme->size = NO_VALUE;
                    lexeme->value[lexeme->size] = '\0';
                    if (character != EOF) {
                        ungetc(character, stdin);
                    }
                }
            } else {
                // Handles one-line quotation
                handle_quotation(character, lexeme, false);
            }
            token.type = T_STRING_LITERAL;
            token.size = lexeme->size;
            strcpy(token.value.string_value, lexeme->value);
            break;

        default:
            break;
        }
        character = getchar();
    } while (token.type == T_UNDEFINED && character != EOF);

    // In case next character is EOF, switch will never go into CASE_SLASH and can't set type to T_DIV, so I have to do it here
    if (character == EOF && current_case == CASE_SLASH) {
        token.type = T_DIV;
    }

    // Return last read character, that was not used
    if (character != EOF) {
        ungetc(character, stdin);
    }
    return token;
} // end of first_special()

token_t get_next_token()
{
    // Initialize token and variable for storing read characters
    token_t token = new_token(current_line_num);
    lexeme_t lexeme = {0};
    scanner_case current_case = CASE_START;
    int character;
    char next_character;

    // Main loop for finding and creating token
    while (token.type == T_UNDEFINED && next_token.type == T_UNDEFINED) {
        // Handle white spaces and if some if them is a token, leave loop
        token = handle_white_spaces(&character, token);
        if (token.type != T_UNDEFINED) {
            break;
        }

        if (isalpha(character) || character == '_') {
            token = first_alpha(character, token, &lexeme);
        } else if (isdigit(character) || character == '.') {
            if (character == '.') {
                if (isdigit(next_character = getchar())) {
                    token = first_digit(character, token, &lexeme, CASE_DOUBLE_LITERAL);
                }
                else {
                    next_token = new_token(T_DOT);
                    ungetc(next_character, stdin);
                }
            }
            else if (character != '0') {
                token = first_digit(character, token, &lexeme, CASE_INT_LITERAL);
            }
            else { error_exit(LEX_ERR, current_line_num, "Non-Zero number starting with zero."); }
        }
        else if (!isspace(character)) {
            token = first_special(character, token, &lexeme, current_case);
        }
    }

    if (next_token.type != T_UNDEFINED) {
        token = next_token;
        next_token = new_token(NO_VALUE);
    }

    handle_indent(&token);

    return token;
}