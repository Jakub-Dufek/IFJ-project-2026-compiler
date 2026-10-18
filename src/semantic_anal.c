/**
 * @file semantic_anal.c
 * @internal Part of IFJ-project-2026-compiler, Team 274855
 *
 * @brief semantic analysis phase of the compiler
 * @author Ondřej Kycl (xkyclon00)
 * @date 17. 10. 2026
 * 
 * @details this file contains implementation of the semantic analyzer of the compiler
 */

//TODO:

#include "semantic_anal.h"
void sem_anal(ast_node_t *ast_node, symtable_bvs_node_t **symtable_root){
    if (ast_node == NULL) {
        return;
    }
    switch (ast_node->stmt_type){
        case AST_DECLARATION:
            sem_anal(ast_node->data->declaration_node.expression, symtable_root);
            symbol_data_t new_symbol;
            new_symbol.data_type = ast_node->data->declaration_node.data_type;
            if (ast_node->data->declaration_node.is_mutable) {
                new_symbol.id_type = ID_VAR; 
            }
            else {
                new_symbol.id_type = ID_VAL;
            }
            // other attributes ....

            *symtable_root = symtable_insert(*symtable_root, ast_node->data->declaration_node.id_val, new_symbol);
            break;
        case EXPR_OPERATION:
            break;
        case EXPR_LITERAL:
            break;
        case EXPR_IDENTIFIER:
            break;
    }
 }



void run_sem_anal(ast_node_t *ast_node) {
    symtable_bvs_node_t *symtable = symtable_initialize();

    sem_anal(ast_node, &symtable);

    symtable_destructor(symtable);
}