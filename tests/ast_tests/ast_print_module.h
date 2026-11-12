#ifndef AST_PRINT_MODULE_H
#define AST_PRINT_MODULE_H

/**
 * @brief Main recursive function for printing the AST structure
 * 
 * @param node Node of top of syntax tree
 * @param level 0 at begginging, otherwise starting indent level
 */
void ast_print(ast_node_t *node, int level);

#endif
