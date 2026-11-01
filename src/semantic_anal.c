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
//      Popsat implicitní konverzi jako AI generated?
//      Volání funkcí: Sedí počet a typy předaných argumentů s definicí funkce? , volání před lexikální definicí?
//      Typ návratové hodnoty u funkce --> ID_VAL?
//      Synchronize Symtable, AST, 
//      error_exit() leaks!!

#include "semantic_anal.h"

data_type sem_anal(ast_node_t *ast_node, symtable_stack_t *stack){
    while (ast_node != NULL) {
        switch (ast_node->stmt_type){
            case AST_DEFINITION: {
                symbol_data_t new_symbol;
                new_symbol.data_type = ast_node->data->definition_node.data_type;
                new_symbol.id_type = ID_FUNC; // <-- Come back to this>ID_VAL
                param_type_list_t *param_type_list = param_type_list_initialize(); // <-- Later destroy this ! 
                unsigned param_counter = 0;
                ast_node_t *parameter = ast_node->data->definition_node.first_param;
                while (parameter != NULL){
                    param_type_list_insert(param_type_list, parameter->data->declaration_node.data_type);
                    param_counter++;
                    parameter = parameter->next;
                }
                new_symbol.func_info.is_defined = true;
                new_symbol.func_info.param_types = param_type_list;
                new_symbol.func_info.param_num = param_counter;
                new_symbol.func_info.is_main = ast_node->data->definition_node.is_main;
                stack->top->symtable = symtable_insert(stack->top->symtable, ast_node->data->definition_node.id_val, new_symbol);

                symtable_stack_push(stack);
                
                parameter = ast_node->data->definition_node.first_param;
                while (parameter != NULL){
                    // Preparing parameter for getting inserted into symtable
                    symbol_data_t new_symbol;
                    new_symbol.data_type = parameter->data->declaration_node.data_type;
                    if (parameter->data->declaration_node.is_mutable) {
                        new_symbol.id_type = ID_VAR; 
                    }
                    else {
                        new_symbol.id_type = ID_VAL;
                    }
                    // other attributes .... <--
                    stack->top->symtable = symtable_insert(stack->top->symtable, parameter->data->declaration_node.id_val, new_symbol);

                    parameter = parameter->next;
                }
                //může být i volání nebo výraz!! -> sem_anal(   node.body->data->block_node    NEBO    node.body->data->expression / node.body->data->func_call   ) 
                sem_anal(ast_node->data->definition_node.body->data->block_node.first, stack);

                symtable_stack_pop(stack);
                break;
            }
            case AST_BLOCK:
                symtable_stack_push(stack);

                sem_anal(ast_node->data->block_node.first, stack);
                
                symtable_stack_pop(stack);
                break;

            case AST_DECLARATION: {
                data_type expr_type = sem_anal(ast_node->data->declaration_node.expression, stack);
                data_type var_type = ast_node->data->declaration_node.data_type;

                // Static type deduction
                if (var_type == T_NO_TYPE){
                    if (expr_type == T_UNDEFINED){
                        error_exit(SEM_TYPE_ERR, ast_node->line_num, "Static type deduction not possible for this expression"); // Jak se dostat sem??
                    }
                    var_type = expr_type;
                    ast_node->data->declaration_node.data_type = var_type;
                }
            
                // Node compatibility check in assignment
                if (var_type == expr_type){
                    //Everything good
                }
                else if (var_type == T_KEY_DOUBLE && expr_type == T_KEY_INT){
                    ast_node->data->declaration_node.expression = ast_convert_int2float(ast_node->data->declaration_node.expression);
                }
                else if (var_type == T_KEY_INT && expr_type == T_KEY_DOUBLE){
                    error_exit(SEM_TYPE_ERR, ast_node->line_num, "Cannot convert Double to Int");
                }
                else{
                    error_exit(SEM_TYPE_ERR, ast_node->line_num, "Incompatible types assignment");
                }
                // Preparation for inserting into symtable
                symbol_data_t new_symbol;
                new_symbol.data_type = ast_node->data->declaration_node.data_type;
                if (ast_node->data->declaration_node.is_mutable) {
                    new_symbol.id_type = ID_VAR; 
                }
                else {
                    new_symbol.id_type = ID_VAL;
                }
                // other attributes .... <--
                stack->top->symtable = symtable_insert(stack->top->symtable, ast_node->data->declaration_node.id_val, new_symbol);
                break;
            }
            case EXPR_OPERATION: {
                data_type left_type = sem_anal(ast_node->data->op_node.left, stack);
                data_type right_type = sem_anal(ast_node->data->op_node.right, stack);

                if (left_type == right_type){
                    return left_type;
                }
                else if (left_type == T_KEY_INT && right_type == T_KEY_DOUBLE){
                    ast_node->data->op_node.left = ast_convert_int2float(ast_node->data->op_node.left);
                    return T_KEY_DOUBLE; 
                }
                else if (left_type == T_KEY_DOUBLE && right_type == T_KEY_INT){
                    ast_node->data->op_node.right = ast_convert_int2float(ast_node->data->op_node.right);
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
                break;

            case EXPR_IDENTIFIER: {
                symtable_bvs_node_t *symtable_node = symtable_stack_search(stack, ast_node->data->id_leaf_node.id_val);
                if(symtable_node == NULL){
                    error_exit(SEM_UNDEF_ERR, ast_node->line_num, "Using non existing identifier");                     // <---- Come back to this
                }
                else{
                    return symtable_node->symbol_data.data_type;
                }
            }
            default:
                break;
        }   
        ast_node = ast_node->next; //Move to next line in block
    }
    return T_NO_TYPE;
 }



void run_sem_anal(ast_node_t *ast_node) {
    symtable_stack_t symtable_stack;
    symtable_stack_initialize(&symtable_stack);

    symtable_stack_push(&symtable_stack);

    sem_anal(ast_node, &symtable_stack);

    symtable_stack_pop(&symtable_stack);

    /*if (sem_anal(ast_node, &symtable) == !!!!!!!!!!-->T_NO_TYPE<--------){
        error_exit(SEM_UNDEF_ERR, 0, "Main function not defined"); //<-- COme back to this
    }*/
    
}