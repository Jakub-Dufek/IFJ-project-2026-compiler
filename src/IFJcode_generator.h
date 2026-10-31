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

void print_string_literal(char *string);

token_type find_operand_type (ast_node_t *node);


void generate_block(ast_node_t *node);

void generate_declaration(ast_node_t *node);

void generate_function(ast_node_t *node);

void generate_operation(ast_node_t *node);

void generate_identifier(ast_node_t *node, char *print_before);

void generate_literal(ast_node_t *node, char *print_before);



void process_node(ast_node_t *node, char *message_before);

void generate_IFJcode26(ast_node_t *root);


#endif