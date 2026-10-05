/**
 * @file structures.h
 * @internal Part of IFJ-project-2026-compiler
 *
 * @brief Library file containing structures
 * @author Šimon Greger (xgreges00)
 * @author Samuel Vít Lehotský (xlehots00)
 * @date 27.09.2026
 * 
 * @details This file contains definitions of structures
 */

#ifndef STRUCTURES_H
#define STRUCTURES_H

#define MAX_LENGTH 256
#define MAX_INDENT_LENGTH 256
#define NO_VALUE 0
#define KEYWORDS_TABLE_NUM_ELEMENTS 22

#include "error.h"

typedef enum token_type {
    T_UNDEFINED = NO_VALUE,

    //type keywords
    T_KEY_INT,
    T_KEY_DOUBLE,
    T_KEY_STRING,
    T_KEY_BOOLEAN,
    T_KEY_UNIT,
    T_NO_TYPE,

    //keywords
    T_KEY_DEF,
    T_KEY_DO,
    T_KEY_ELSE,
    T_KEY_IF,
    T_KEY_IMPORT,
    T_KEY_RETURN,
    T_KEY_THEN,
    T_KEY_VAL,
    T_KEY_VAR,
    T_KEY_WHILE,
    T_KEY_CASE,
    T_KEY_CLASS,
    T_KEY_EXTENSION,
    T_KEY_FOR,
    T_KEY_LIST,
    T_KEY_MATCH,
    T_KEY_TYPE,
    T_KEY_TRUE,
    T_KEY_FALSE,

    //operators
    T_PLUS,     // '+'
    T_MINUS,    // '-'
    T_MULT,     // '*'
    T_DIV,      // '/'
    T_GT,       // '>'
    T_GTE,      // '>='
    T_LT,       // '<'
    T_LTE,      // '<='
    T_EQ,       // '=='
    T_NEQ,      // '!='
    T_ASSIGN,   // '='
    
    T_DOT,              // '.'
    T_L_ROUND_BRACKET,  // '('
    T_R_ROUND_BRACKET,  // ')'

    T_LF,           // '\n'
    T_COLON,        // ':'
    T_COMMA,        // ','
    T_SEMICOLON,    // ';'
    T_EOF,          // 'EOF'
    T_AT_MAIN,      // "@main"
    T_MORE_INDENT,
    T_LESS_INDENT,

    T_IDENTIFIER,
    T_STRING_LITERAL,
    T_INT_LITERAL,
    T_DOUBLE_LITERAL,
    T_UNIT_LITERAL
} token_type;

typedef struct keywords_dictionary {
    char* string;
    token_type t_type;
} keywords;

keywords keywords_table[KEYWORDS_TABLE_NUM_ELEMENTS] = {
    {"Boolean", T_KEY_BOOLEAN}, 
    {"Double", T_KEY_DOUBLE},
    {"Int", T_KEY_INT},
    {"List", T_KEY_LIST},
    {"String", T_KEY_STRING},
    {"Unit", T_KEY_UNIT},
    {"case", T_KEY_CASE},
    {"class", T_KEY_CLASS},
    {"def", T_KEY_DEF},
    {"do", T_KEY_DO},
    {"else", T_KEY_ELSE},
    {"extension", T_KEY_EXTENSION},
    {"for", T_KEY_FOR},
    {"if", T_KEY_IF},
    {"import", T_KEY_IMPORT},
    {"match", T_KEY_MATCH},
    {"return", T_KEY_RETURN},
    {"then", T_KEY_THEN},
    {"type", T_KEY_TYPE},
    {"val", T_KEY_VAL},
    {"var", T_KEY_VAR},
    {"while", T_KEY_WHILE}
};

/**
 * @union token_value_t
 * @brief Union containing value of the token
 * 
 * This union holds the value of the token, depending on the type of the value
 */
typedef union Token_value {
    int int_value;                  /**<Storage for integer type of the token */
    double double_value;            /**<Storage for double type of the token */
    char string_value[MAX_LENGTH];  /**<Storage for string representation of the token */
} token_value_t;

/**
 * @struct token_t
 * @brief Structure representing one token
 * 
 * This structure holds value of the token, its size and its type
 */
typedef struct Token {
    token_type type;        /**<Type of the token */
    unsigned line_num;      /**<Line on which the token was found */
    token_value_t value;    /**<Specific lexeme represented by this token, that was read from input */
    unsigned size;          /**<Size of the token */
} token_t;


#endif