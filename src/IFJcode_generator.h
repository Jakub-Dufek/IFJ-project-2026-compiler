/**
 * @file IFJcode_generator.h
 * @internal Part of IFJ-project-2026-compiler
 *
 * @brief Code generator library file
 * @author Šimon Greger (xgreges00)
 * @date 25.10.2026
 *
 * @details This file contains definitions of functions used in IFJcode_generator.c
 */

#ifndef IFJCODE_GENERATOR_H
#define IFJCODE_GENERATOR_H

#include "ast.h"

typedef struct Context {
    char *function_name;
    token_type return_type;
    ast_node_t *root;
    int label_idx;
} context_t;


/**
 * @brief Prints string literal to stdout in IFJcode26 style
 *
 * @param string String representing the literal for print
 */
void print_string_literal(char *string, bool flag);

/**
 * @brief Discoveres the type of inputed node
 *
 * @param node Pointer to the node we want to know the type of
 * 
 * @return Type of the inputed node
 */
token_type find_operand_type (ast_node_t *node);

/**
 * TODO
 */
void generate_if(ast_node_t *node, context_t *context);

/**
 * TODO
 */
void generate_while(ast_node_t *node, context_t *context);

/**
 * @brief Generates IFJcode26 for one block of code
 *
 * @param node Pointer to a node that stores the start of a block
 * @param result_needed Boolean indicating if the result of the block is needed
 */
void generate_block(ast_node_t *node, context_t *context, bool result_needed);

/**
 * @brief Generates IFJcode26 for a declaration of a variable
 *
 * @param node Pointer to a node with the variable declaration
 * @param frame String representing the frame in which the variable should be (TF, LF, GF)
 */
void generate_declaration(ast_node_t *node, char *frame, context_t *context);

/**
 * TODO
 */
void generate_assignment(ast_node_t *node, context_t *context);

/**
 * @brief Generates IFJcode26 for a definition of a function
 *
 * @param node Pointer to a node that is holding the definition of a function
 */
void generate_function(ast_node_t *node, context_t *context);

/**
 * TODO
 */
void generate_return(ast_node_t *node, context_t *context);

/**
 * @brief Checks if a node represents an expression, aka. if it leaves a value on the stack
 *
 * @param node Pointer to the node strucutre to check
 * @return true if the node represents an expression, false otherwise
 */
bool is_expression (ast_node_t *node, context_t *context);

/**
 * TODO
 */
token_type function_return_type(char *id_val, context_t *context);

/**
 * TODO
 */
void generate_call(ast_node_t *node, context_t *context);

/**
 * @brief Generates IFJcode26 for an aritmetical operation
 *
 * @param node Pointer to a node that stores the operation
 */
void generate_operation(ast_node_t *node, context_t *context);

/**
 * TODO
 */
void generate_identifier(ast_node_t *node, char *print_before, context_t *context);

/**
 * @brief Generates IFJcode26 for any type of literal
 *
 * @param node Pointer to a node that holds the literal
 * @param print_before String with the information that should be printed before the literal. If empty, is NULL
 */
void generate_literal(ast_node_t *node, char *print_before, context_t *context);


/**
 * @brief Crossroad that detects the type of the node and calls functions for IFJcode26 generation
 *
 * @param node Pointer to a node
 * @param message_before String with the information that should be printed before anything else. If empty, is NULL
 */
void process_node(ast_node_t *node, char *message_before, context_t *context);

void generate_print(ast_node_t *node);

void generate_readLine(char *target_var);

void generate_readInt(char *target_var);

void generate_readDouble(char *target_var); 

void generate_Boolean(char *target_var);

void generate_toStr(char *target_var, ast_node_t *node);

void generate_toInt(char *target_var, ast_node_t *node);

void generate_strLength(char *target_var, ast_node_t *node);

void generate_subStr(ast_node_t *node);

void generate_builtin_subStr();

void generate_ord(char *target_var, ast_node_t *node);

void generate_chr(char *target_var, ast_node_t *node);

/**
 * TODO
 */
char *find_main_function(ast_node_t *root);

/**
 * @brief Entry function for this module
 *
 * @param root Pointer to the root of the semantic tree that should be compiled into IFJcode26
 */
void generate_IFJcode26(ast_node_t *root);


#endif