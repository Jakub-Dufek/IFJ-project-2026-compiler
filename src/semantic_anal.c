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
//      Jak tu přesně funguje symtable?
//      Implicitní konverze
//      Volání funkcí: Sedí počet a typy předaných argumentů s definicí funkce?
//      Typ návratové hodnoty u funkce
//      Automatické přiřazení typu, když není zadán? (možná to dělá už syntax)
//      Synchronize Symtable, AST, 

#include "semantic_anal.h"

data_type sem_anal(ast_node_t *ast_node, symtable_bvs_node_t **symtable_root){
    if (ast_node == NULL) {
        return T_NO_TYPE;
    }
    switch (ast_node->stmt_type){
        case AST_DECLARATION: {
            data_type expr_type = sem_anal(ast_node->data->declaration_node.expression, symtable_root);
            data_type var_type = ast_node->data->declaration_node.data_type;

            if (var_type == expr_type){

            }
            else if (var_type == T_KEY_DOUBLE && expr_type == T_KEY_INT){

            }
            else if (var_type == T_KEY_INT && expr_type == T_KEY_DOUBLE){
                error_exit(SEM_TYPE_ERR, ast_node->line_num, "Cannot convert Double to Int");
            }
            else{
                error_exit(SEM_TYPE_ERR, ast_node->line_num, "Incompatible types assignment");
            }
            symbol_data_t new_symbol;
            new_symbol.data_type = ast_node->data->declaration_node.data_type;
            if (ast_node->data->declaration_node.is_mutable) {
                new_symbol.id_type = ID_VAR; 
            }
            else {
                new_symbol.id_type = ID_VAL;
            }
            // other attributes .... <--

            *symtable_root = symtable_insert(*symtable_root, ast_node->data->declaration_node.id_val, new_symbol);
            break;
        }
        case EXPR_OPERATION: {
            data_type left_type = sem_anal(ast_node->data->op_node.left, symtable_root);
            data_type right_type = sem_anal(ast_node->data->op_node.right, symtable_root);

            if (left_type == right_type){
                return left_type;
            }
            else if ((left_type == T_KEY_DOUBLE && right_type == T_KEY_INT)
                    || (left_type == T_KEY_INT && right_type == T_KEY_DOUBLE))              
            {
                return T_KEY_DOUBLE; 
            }
            else {
                error_exit(SEM_TYPE_ERR, ast_node->line_num, "Types of operation not matching"); // <-- Come back to this
            }
            break; 
        }
        case EXPR_LITERAL:
            int lit_type = ast_node->data->lit_leaf_node.lit_type;
            if (lit_type == T_DOUBLE_LITERAL){
                return T_KEY_DOUBLE;
            }
            else if (lit_type == T_INT_LITERAL){
                return T_KEY_INT;
            }
            else if (lit_type == T_STRING_LITERAL){
                return T_KEY_STRING;
            }
            else if (lit_type == T_UNIT_LITERAL){
                error_exit(SEM_TYPE_ERR, ast_node->line_num, "Cannot use Unit type in an expression");
            }
            else {                  // Maybe Boolean?
                return lit_type;
            }

        case EXPR_IDENTIFIER: {
            symtable_bvs_node_t *symtable_node;
            if((symtable_node = symtable_find_symbol(*symtable_root, ast_node->data->id_leaf_node.id_val)) == NULL){
                error_exit(SEM_UNDEF_ERR, ast_node->line_num, "Using non existing identifier");                     // <---- Come back to this
            }
            else{
                return symtable_node->symbol_data.data_type;
            }
        }
        default:
            break;
    }
    return T_NO_TYPE;
 }



void run_sem_anal(ast_node_t *ast_node) {
    symtable_bvs_node_t *symtable = symtable_initialize();

    sem_anal(ast_node, &symtable);
    /*if (sem_anal(ast_node, &symtable) == T_NO_TYPE){
        error_exit(SEM_UNDEF_ERR, 0, "Main function not defined"); //<-- COme back to this
    }*/

    symtable_destructor(symtable);
}