/**
* @file ast.h
* @internal Part of IFJ-project-2026-compiler, Team 274855
*
* @brief Header file for Abstract Syntax Tree building
* @author Jakub Dufek (274855)
* @date 3. 10. 2026
*/

#ifndef AST_H
#define AST_H


/**
 * TODO: comment
 */
typedef union node_data {
    // TODO: node_data union
} ast_node_data_t;


/**
 * TODO: comment
 */
typedef struct node {
    int type;
    int line_num;
    ast_node_data_t *data;
    struct node *next;
} ast_node_t;

/**
 * TODO: comment
 */
void ast_insert_var_declaration();


#endif
