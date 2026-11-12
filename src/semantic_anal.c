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

void sem_pre_anal(ast_node_t *ast_node, symtable_stack_t *stack){

    bool main_found = false;
    while (ast_node != NULL && ast_node->stmt_type == AST_DEFINITION){
        if (symtable_find_symbol(stack->top->symtable, ast_node->data->definition_node.id_val) != NULL){
            error_exit(SEM_REDEF_ERR, ast_node->line_num, "SEMANTIC ANALYSIS: Redefinition of a function is not allowed!");
        }
        if (ast_node->data->definition_node.is_main == true){
            if (main_found == true){
                error_exit(SEM_REDEF_ERR, ast_node->line_num, "SEMANTIC ANALYSIS: Multiple @main functions are not allowed!");
            }
            main_found = true;

            if (ast_node->data->definition_node.first_param != NULL){
                error_exit(SEM_ARG_ERR, ast_node->line_num, "SEMANTIC ANALYSIS: There cannot be any arguments in @main function!");
            }
            if (ast_node->data->definition_node.is_main == false){
                error_exit(SEM_UNDEF_ERR, ast_node->line_num, "SEMANTIC ANALYSIS: Missing @main function!");
            }
        }

        symbol_data_t new_symbol = {0};
        new_symbol.data_type = ast_node->data->definition_node.data_type;
        new_symbol.id_type = ID_FUNC;
        param_type_list_t *param_type_list = param_type_list_initialize(); // <-- Later destroy this ! 
        unsigned param_counter = 0;
        ast_node_t *parameter = ast_node->data->definition_node.first_param;
        while (parameter != NULL){
            param_type_list_insert(param_type_list, parameter->data->param_node.data_type);
            param_counter++;
            parameter = parameter->next;
        }
        new_symbol.func_info.is_defined = true;
        new_symbol.func_info.param_types = param_type_list;
        new_symbol.func_info.param_num = param_counter;
        new_symbol.func_info.is_main = ast_node->data->definition_node.is_main;
        stack->top->symtable = symtable_insert(stack->top->symtable, ast_node->data->definition_node.id_val, new_symbol);

        ast_node = ast_node->next;
    }
    if (main_found == false){
        error_exit(SEM_UNDEF_ERR, 0, "SEMANTIC ANALYSIS: Missing @main function!");
    }
}

data_type sem_anal(ast_node_t *ast_node, symtable_stack_t *stack){
    data_type curr_func_ret_type; // current function return type helper variable

    while (ast_node != NULL) {
        switch (ast_node->stmt_type){
            case AST_DEFINITION: {
                symtable_stack_push(stack);

                ast_node_t *parameter = ast_node->data->definition_node.first_param;
                while (parameter != NULL){
                    // Preparing parameter for getting inserted into symtable
                    symbol_data_t new_symbol = {0};
                    new_symbol.data_type = parameter->data->param_node.data_type;
                    new_symbol.id_type = ID_VAL;
                    if (symtable_find_symbol(stack->top->symtable, parameter->data->param_node.id_val) != NULL){
                        error_exit(SEM_REDEF_ERR, ast_node->line_num, "SEMANTIC ANALYSIS: Duplicate parameter names!");
                    }
                    stack->top->symtable = symtable_insert(stack->top->symtable, parameter->data->param_node.id_val, new_symbol);
                    parameter = parameter->next;
                }

                curr_func_ret_type = ast_node->data->definition_node.data_type; 
                data_type body_type = sem_anal(ast_node->data->definition_node.body, stack); //<-- Body entry
                if (ast_node->data->definition_node.body->stmt_type != AST_BLOCK){
                    if (curr_func_ret_type == body_type || curr_func_ret_type == T_KEY_UNIT) {

                    }
                    else if (curr_func_ret_type == T_KEY_DOUBLE && body_type == T_KEY_INT){
                        ast_node->data->definition_node.body = ast_convert_int2float(ast_node->data->definition_node.body);
                    }
                    else{
                        error_exit(SEM_TYPE_ERR, ast_node->line_num, "SEMANTIC ANALYSIS: Return type mismatch in single-line function body");
                    }
                }

                symtable_stack_pop(stack);
                break;
            }
            case AST_CALL: {
                symtable_bvs_node_t *func_symbol = symtable_stack_search(stack, ast_node->data->call_node.id_val);
                if (func_symbol == NULL || func_symbol->symbol_data.id_type != ID_FUNC){
                    error_exit(SEM_VAR_ERR, ast_node->line_num, "SEMANTIC_ANALYSIS: Attempted to call undefined function");
                }

                ast_node_t *curr_arg = ast_node->data->call_node.first_arg;
                param_type_t *expected_param = func_symbol->symbol_data.func_info.param_types->first;  
                unsigned func_param_counter = 0;

                while(curr_arg != NULL && expected_param != NULL){
                    data_type curr_arg_type = sem_anal(curr_arg, stack);
                    if (/*expected_param->type != T_ANY &&*/ curr_arg_type != expected_param->type){
                        error_exit(SEM_ARG_ERR, ast_node->line_num, "SEMANTIC ANALYSIS: Argument types do not match!");
                    }
                    curr_arg = curr_arg->next;
                    expected_param = expected_param->next_ptr;
                    func_param_counter++;
                }
                if (func_param_counter != func_symbol->symbol_data.func_info.param_num){
                    error_exit(SEM_ARG_ERR, ast_node->line_num, "SEMANTIC ANALYSIS: Number of parameters doesn't match!");
                }
                return func_symbol->symbol_data.data_type;
            }
            case AST_BLOCK: {
                symtable_stack_push(stack);

                sem_anal(ast_node->data->block_node.first, stack);
                
                symtable_stack_pop(stack);
                break;
            }
            case AST_WHILE: {
                data_type cond_type = sem_anal(ast_node->data->while_node.condition, stack);
                if (cond_type != T_KEY_BOOLEAN){
                    error_exit(SEM_OTHER_ERR, ast_node->line_num, "SEMANTIC ANALYSIS: not returning boolean as a result of 'while' condition!");
                }

                sem_anal(ast_node->data->while_node.body, stack);
                break;
            }
            case AST_IF: {
                data_type cond_type = sem_anal(ast_node->data->if_node.condition, stack);
                if (cond_type != T_KEY_BOOLEAN){
                    error_exit(SEM_OTHER_ERR, ast_node->line_num, "SEMANTIC ANALYSIS: not returning Boolean as a result of 'while' condition!");
                }
            
                sem_anal(ast_node->data->if_node.then_body, stack);
                if (ast_node->data->if_node.else_body != NULL){
                    sem_anal(ast_node->data->if_node.else_body, stack);
                }
                break;
            }
            case AST_DECLARATION: {
                data_type expr_type = sem_anal(ast_node->data->declaration_node.expr_or_call, stack);
                data_type var_type = ast_node->data->declaration_node.data_type;

                // Static type deduction
                if (var_type == T_NO_TYPE){
                    if (expr_type == T_UNDEFINED){ // Just to be sure
                        error_exit(SEM_TYPE_ERR, ast_node->line_num, "SEMANTIC ANALYSIS: Static type deduction not possible for this expression or call");
                    }
                    if (expr_type == T_KEY_UNIT){
                        error_exit(SEM_TYPE_ERR, ast_node->line_num, "SEMANTIC ANALYSIS: Static type deduction doesn't allow Unit type!");
                    }
                    var_type = expr_type;
                    ast_node->data->declaration_node.data_type = var_type;
                }
            
                // Node compatibility check in assignment
                if (var_type == expr_type){
                    //Everything good
                }
                else if (var_type == T_KEY_DOUBLE && expr_type == T_KEY_INT){
                    ast_node->data->declaration_node.expr_or_call = ast_convert_int2float(ast_node->data->declaration_node.expr_or_call);
                }
                else{
                    error_exit(SEM_TYPE_ERR, ast_node->line_num, "SEMANTIC ANALYSIS: Incompatible types assignment in declaration");
                }
                // Preparation for inserting into symtable
                symbol_data_t new_symbol = {0};
                new_symbol.data_type = ast_node->data->declaration_node.data_type;
                new_symbol.id_type = ast_node->data->declaration_node.is_mutable ? ID_VAR : ID_VAL;
                // Redefinition check before insert
                if (symtable_find_symbol(stack->top->symtable, ast_node->data->declaration_node.id_val) != NULL){
                    error_exit(SEM_REDEF_ERR, ast_node->line_num, "SEMANTIC ANALYSIS: Redefinition of a variable!");
                }
                stack->top->symtable = symtable_insert(stack->top->symtable, ast_node->data->declaration_node.id_val, new_symbol);
                break;
            }
            case AST_ASSIGN: {
                symtable_bvs_node_t *symbol = symtable_stack_search(stack, ast_node->data->assign_node.id_val);
                if (symbol == NULL){
                    error_exit(SEM_UNDEF_ERR, ast_node->line_num, "SEMANTIC ANALYSIS: Non existing variable!");
                }
                if (symbol->symbol_data.id_type == ID_VAL || symbol->symbol_data.id_type == ID_FUNC){
                    error_exit(SEM_VAR_ERR, ast_node->line_num, "SEMANTIC_ANALYSIS: Attempted to assign value to a non VAR symbol!");
                }
                // Compare data type against assigned expression or call
                data_type expr_or_call_type = sem_anal(ast_node->data->assign_node.expr_or_call, stack);
                if (symbol->symbol_data.data_type == expr_or_call_type){

                }
                else if (symbol->symbol_data.data_type == T_KEY_DOUBLE && expr_or_call_type == T_KEY_INT){
                    ast_node->data->assign_node.expr_or_call = ast_convert_int2float(ast_node->data->assign_node.expr_or_call);
                }
                else{
                    error_exit(SEM_TYPE_ERR, ast_node->line_num, "SEMANTIC ANALYSIS: Incompatible types assignment");
                }
                break;
            }
            case EXPR_OPERATION: {
                data_type left_type = sem_anal(ast_node->data->op_node.left, stack);
                data_type right_type = sem_anal(ast_node->data->op_node.right, stack);
                token_type op = ast_node->data->op_node.op_type;

                // Implicit conversion INT --> DOUBLE
                if (left_type == T_KEY_INT && right_type == T_KEY_DOUBLE){
                    ast_node->data->op_node.left = ast_convert_int2float(ast_node->data->op_node.left);
                    left_type = T_KEY_DOUBLE; 
                }
                else if (left_type == T_KEY_DOUBLE && right_type == T_KEY_INT){
                    ast_node->data->op_node.right = ast_convert_int2float(ast_node->data->op_node.right);
                    right_type = T_KEY_DOUBLE;
                }
                // Check by operator type
                if (op == T_PLUS || op == T_MINUS || op == T_MULT || op == T_DIV){
                    if (left_type == T_KEY_STRING && right_type == T_KEY_STRING && op == T_PLUS) {
                        return T_KEY_STRING; // String concatenation
                    }
                    if (left_type == right_type && (left_type == T_KEY_INT || left_type == T_KEY_DOUBLE)){
                        return left_type;
                    }
                    error_exit(SEM_TYPE_ERR, ast_node->line_num, "Invalid operands for arithmetics");
                }
                
                if (op == T_LT || op == T_LTE || op == T_GT || op == T_GTE || op == T_EQ || op == T_NEQ){
                    if (left_type != right_type){
                        error_exit(SEM_TYPE_ERR, ast_node->line_num, "Invalid data types comparison in 'if' condition");
                    }
                    return T_KEY_BOOLEAN;
                }
                error_exit(SEM_OTHER_ERR, ast_node->line_num, "Unknown operator in 'if' condition check");
                break; 
            }
            case EXPR_LITERAL: {
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
                    return T_KEY_UNIT;
                }
                else {                  // Maybe Boolean?
                    return lit_type;
                }
                break;
            }
            case EXPR_IDENTIFIER: {
                symtable_bvs_node_t *symtable_node = symtable_stack_search(stack, ast_node->data->id_leaf_node.id_val);
                if(symtable_node == NULL){
                    error_exit(SEM_UNDEF_ERR, ast_node->line_num, "Using non existing identifier");                     // <---- Come back to this
                }
                else{
                    ast_node->data->id_leaf_node.symbol_data = symtable_node->symbol_data;

                    return symtable_node->symbol_data.data_type;
                }
            }
            case AST_RETURN: {
                data_type ret_expr_type;
                if (ast_node->data->return_node.expr_or_call == NULL){
                    ret_expr_type = T_KEY_UNIT;
                }
                else {
                    ret_expr_type = sem_anal(ast_node->data->return_node.expr_or_call, stack);
                }
                // Compare against expected function return type
                if (curr_func_ret_type == ret_expr_type){
                    // all good
                }
                else if (curr_func_ret_type == T_KEY_DOUBLE && ret_expr_type == T_KEY_INT){
                    ast_node->data->return_node.expr_or_call = ast_convert_int2float(ast_node->data->return_node.expr_or_call);
                }
                else{
                    error_exit(SEM_TYPE_ERR, ast_node->line_num, "SEMANTIC ANALYSIS: Expected different data type to be returned");
                }
                break;
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

    sem_pre_anal(ast_node, &symtable_stack);
    sem_anal(ast_node, &symtable_stack);
    
    symtable_stack_pop(&symtable_stack);
}