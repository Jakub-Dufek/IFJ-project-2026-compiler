/**
* @file structures.h
* @internal Part of IFJ-project-2026-compiler
*
* @brief Library file containing structures
* @author Šimon Greger (xgreges00)
* @date 27.09.2026
* 
* @details This file contains definitions of structures
*/

#ifndef STRUCTURES
#define STRUCTURES

//LIBRARIES
#include <stdio.h>


typedef enum {
    //keywords
    T_KEY_DEF,
    T_KEY_DO,
    T_KEY_DOUBLE,
    T_KEY_ELSE,
    T_KEY_IF,
    T_KEY_IMPORT,
    T_KEY_INT,
    T_KEY_RETURN,
    T_KEY_STRING,
    T_KEY_THEN,
    T_KEY_UNIT,
    T_KEY_VAL,
    T_KEY_VAR,
    T_KEY_WHILE,
    T_KEY_BOOLEAN,
    T_KEY_CASE,
    T_KEY_CLASS,
    T_KEY_EXTENSION,
    T_KEY_FOR,
    T_KEY_LIST,
    T_KEY_MATCH,
    T_KEY_TYPE,

    //operators
    T_PLUS,
    T_MINUS,
    T_MULT,
    T_DIV,
    T_GT,   //greater than
    T_GTE,  //greater or equal than
    T_LT,   //lesser than
    T_LTE,  //lesser or equal than
    T_EQ,   //equal
    T_NEQ,  //not equal
    T_ASSIGN,
    
    T_DOT,
    T_LROUNDBRACKET,
    T_RROUNDBRACKET,

    T_LF,
    T_COLON,
    T_COMMA,
    T_SEMICOLON,
    T_EOF,
    T_AT_MAIN,  //@main

    T_IDENTIFIER,
    T_STRING_LITERAL,
    T_INT_LITERAL,
    T_DOUBLE_LITERAL,
    T_UNIT_LITERAL
} token_type;

/**
 * @struct lexeme_t
 * @brief Structure representing one lexeme
 * 
 * This structure holds value of the lexeme and its length
 */
typedef struct Lexeme {
    char* value;    /**<String representation of the lexeme */
    size_t size;    /**<Size of the lexeme */
} lexeme_t;

/**
 * @union token_value_t
 * @brief Union containing value of the token
 * 
 * This union holds the value of the token, depending on the type of the value
 */
typedef union Token_value {
    int int_value;          /**<Storage for integer type of the token */
    double double_value;    /**<Storage for double type of the token */
    char* string_value;     /**<Storage for string representation of the token */
} token_value_t;

/**
 * @struct token_t
 * @brief Structure representing one token
 * 
 * This structure holds value of the token, its size and its type
 */
typedef struct Token {
    token_type type;        /**<Type of the token */
    token_value_t value;    /**<String representation of the token */
    size_t size;            /**<Size of the token */
} token_t;

#endif