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
//      valgrind?
//      T_KEY_BOOLEAN >> T_LITERAL_BOOLEAN
//      Underscore "_"

#include "semantic_anal.h"

static data_type curr_func_ret_type; // current function return type helper variable
static symtable_stack_t *active_sem_stack = NULL;
static unsigned unique_symbol_counter = 1;

// BUILT-IN FUNCTIONS INSERTION

void builtin_functions_insertion(symtable_stack_t *stack){
    print_insertion(stack);
    readline_insertion(stack);
    readint_insertion(stack);
    readdouble_insertion(stack);
    tostr_insertion(stack);
    toint_insertion(stack);
    strlength_insertion(stack);
    substr_insertion(stack);
    ord_insertion(stack);
    chr_insertion(stack);
    readboolean_insertion(stack);
}

void print_insertion(symtable_stack_t *stack){
    symbol_data_t symbol = {0};
    param_type_list_t *params = param_type_list_initialize();

    symbol.id_type = ID_FUNC;
    symbol.data_type = T_KEY_UNIT;
    symbol.func_info.is_defined = true; 
    symbol.func_info.is_main = false;
    symbol.func_info.param_num = 1;
    param_type_list_insert(params, T_ANY_TYPE);
    symbol.func_info.param_types = params;

    stack->top->symtable = symtable_insert(stack->top->symtable, "print", symbol);
}
void readline_insertion(symtable_stack_t *stack){
    symbol_data_t symbol = {0};
    param_type_list_t *params = param_type_list_initialize();

    symbol.id_type = ID_FUNC;
    symbol.data_type = T_KEY_STRING;
    symbol.func_info.is_defined = true; 
    symbol.func_info.is_main = false;
    symbol.func_info.param_num = 0;
    symbol.func_info.param_types = params;

    stack->top->symtable = symtable_insert(stack->top->symtable, "readLine", symbol);
}
void readint_insertion(symtable_stack_t *stack){
    symbol_data_t symbol = {0};
    param_type_list_t *params = param_type_list_initialize();

    symbol.id_type = ID_FUNC;
    symbol.data_type = T_KEY_INT;
    symbol.func_info.is_defined = true; 
    symbol.func_info.is_main = false;
    symbol.func_info.param_num = 0;
    symbol.func_info.param_types = params;

    stack->top->symtable = symtable_insert(stack->top->symtable, "readInt", symbol);
}
void readdouble_insertion(symtable_stack_t *stack){
    symbol_data_t symbol = {0};
    param_type_list_t *params = param_type_list_initialize();

    symbol.id_type = ID_FUNC;
    symbol.data_type = T_KEY_DOUBLE;
    symbol.func_info.is_defined = true; 
    symbol.func_info.is_main = false;
    symbol.func_info.param_num = 0;
    symbol.func_info.param_types = params;

    stack->top->symtable = symtable_insert(stack->top->symtable, "readDouble", symbol);
}
void tostr_insertion(symtable_stack_t *stack){
    symbol_data_t symbol = {0};
    param_type_list_t *params = param_type_list_initialize();

    symbol.id_type = ID_FUNC;
    symbol.data_type = T_KEY_STRING;
    symbol.func_info.is_defined = true; 
    symbol.func_info.is_main = false;
    symbol.func_info.param_num = 1;
    param_type_list_insert(params, T_ANY_TYPE);
    symbol.func_info.param_types = params;

    stack->top->symtable = symtable_insert(stack->top->symtable, "toStr", symbol);
}
void toint_insertion(symtable_stack_t *stack){
    symbol_data_t symbol = {0};
    param_type_list_t *params = param_type_list_initialize();

    symbol.id_type = ID_FUNC;
    symbol.data_type = T_KEY_INT;
    symbol.func_info.is_defined = true; 
    symbol.func_info.is_main = false;
    symbol.func_info.param_num = 1;
    param_type_list_insert(params, T_KEY_DOUBLE);
    symbol.func_info.param_types = params;

    stack->top->symtable = symtable_insert(stack->top->symtable, "toInt", symbol);
}
void strlength_insertion(symtable_stack_t *stack){
    symbol_data_t symbol = {0};
    param_type_list_t *params = param_type_list_initialize();

    symbol.id_type = ID_FUNC;
    symbol.data_type = T_KEY_INT;
    symbol.func_info.is_defined = true; 
    symbol.func_info.is_main = false;
    symbol.func_info.param_num = 1;
    param_type_list_insert(params, T_KEY_STRING);
    symbol.func_info.param_types = params;

    stack->top->symtable = symtable_insert(stack->top->symtable, "strLength", symbol);
}
void substr_insertion(symtable_stack_t *stack){
    symbol_data_t symbol = {0};
    param_type_list_t *params = param_type_list_initialize();

    symbol.id_type = ID_FUNC;
    symbol.data_type = T_KEY_STRING;
    symbol.func_info.is_defined = true; 
    symbol.func_info.is_main = false;
    symbol.func_info.param_num = 3;
    param_type_list_insert(params, T_KEY_STRING);
    param_type_list_insert(params, T_KEY_INT);
    param_type_list_insert(params, T_KEY_INT);
    symbol.func_info.param_types = params;

    stack->top->symtable = symtable_insert(stack->top->symtable, "subStr", symbol);
}
void ord_insertion(symtable_stack_t *stack){
    symbol_data_t symbol = {0};
    param_type_list_t *params = param_type_list_initialize();

    symbol.id_type = ID_FUNC;
    symbol.data_type = T_KEY_INT;
    symbol.func_info.is_defined = true; 
    symbol.func_info.is_main = false;
    symbol.func_info.param_num = 2;
    param_type_list_insert(params, T_KEY_STRING);
    param_type_list_insert(params, T_KEY_INT);
    symbol.func_info.param_types = params;

    stack->top->symtable = symtable_insert(stack->top->symtable, "ord", symbol);
}
void chr_insertion(symtable_stack_t *stack){
    symbol_data_t symbol = {0};
    param_type_list_t *params = param_type_list_initialize();

    symbol.id_type = ID_FUNC;
    symbol.data_type = T_KEY_STRING;
    symbol.func_info.is_defined = true; 
    symbol.func_info.is_main = false;
    symbol.func_info.param_num = 1;
    param_type_list_insert(params, T_KEY_INT);
    symbol.func_info.param_types = params;

    stack->top->symtable = symtable_insert(stack->top->symtable, "chr", symbol);
}
void readboolean_insertion(symtable_stack_t *stack){
    symbol_data_t symbol = {0};
    param_type_list_t *params = param_type_list_initialize();

    symbol.id_type = ID_FUNC;
    symbol.data_type = T_KEY_BOOLEAN;
    symbol.func_info.is_defined = true; 
    symbol.func_info.is_main = false;
    symbol.func_info.param_num = 0;
    symbol.func_info.param_types = params;

    stack->top->symtable = symtable_insert(stack->top->symtable, "readBoolean", symbol);
}


// FIRST GO THROUGH AST

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
            if (ast_node->data->definition_node.data_type != T_KEY_UNIT){
                error_exit(SEM_TYPE_ERR, ast_node->line_num, "SEMANTIC ANALYSIS: @main function must return Unit!");
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


// SECOND GO THROUGH AST

static data_type sem_check_definition(ast_node_t *ast_node, symtable_stack_t *stack) {
    symtable_stack_push(stack);

    ast_node_t *parameter = ast_node->data->definition_node.first_param;
    while (parameter != NULL){
        // Preparing parameter for getting inserted into symtable
        symbol_data_t new_symbol = {0};
        new_symbol.unique_id = unique_symbol_counter++;
        new_symbol.data_type = parameter->data->param_node.data_type;
        new_symbol.id_type = ID_VAL;
        

        if (symtable_find_symbol(stack->top->symtable, parameter->data->param_node.id_val) != NULL){
            error_exit(SEM_REDEF_ERR, ast_node->line_num, "SEMANTIC ANALYSIS: Duplicate parameter names!");
        }
        stack->top->symtable = symtable_insert(stack->top->symtable, parameter->data->param_node.id_val, new_symbol);
        parameter->data->param_node.symbol_data = new_symbol;

        parameter = parameter->next;
    }

    curr_func_ret_type = ast_node->data->definition_node.data_type; 
    data_type body_type = sem_anal(ast_node->data->definition_node.body, stack); //<-- Body entry

    if (ast_node->data->definition_node.body->stmt_type != AST_BLOCK){
        if (curr_func_ret_type == body_type || curr_func_ret_type == T_KEY_UNIT 
            || curr_func_ret_type == T_NO_TYPE) {
            //OK
        }
        else if (curr_func_ret_type == T_KEY_DOUBLE && body_type == T_KEY_INT){
            ast_node->data->definition_node.body = ast_convert_int2float(ast_node->data->definition_node.body);
        }
        else{
            error_exit(SEM_TYPE_ERR, ast_node->line_num, "SEMANTIC ANALYSIS: Return type mismatch in single-line function body");
        }
    }

    symtable_stack_pop(stack);

    // missing funcion type deduction after body is examined
    if (curr_func_ret_type == T_NO_TYPE) {
        ast_node->data->definition_node.data_type = body_type; // update AST
        
        // update in symtable
        symtable_bvs_node_t *func_sym = symtable_find_symbol(stack->top->symtable, ast_node->data->definition_node.id_val);
        if (func_sym == NULL) {
            error_exit(SEM_OTHER_ERR, ast_node->line_num, "SEMANTIC_ANALYSIS: Cannot find a funcion id"); // shouldn'd happen
        }
        func_sym->symbol_data.data_type = body_type; // deduction
    }

    return T_KEY_UNIT;
}

static data_type sem_check_return(ast_node_t *ast_node, symtable_stack_t *stack) {
    data_type ret_expr_type;

    if (curr_func_ret_type == T_NO_TYPE) {
        error_exit(SEM_TYPE_ERR, ast_node->line_num, "SEMANTIC ANALYSIS: 'return' used in function without explicit return type!");
    }

    if (ast_node->data->return_node.expr_or_call == NULL){
        ret_expr_type = T_KEY_UNIT;
    }
    else {
        ret_expr_type = sem_anal(ast_node->data->return_node.expr_or_call, stack);
    }
    // Compare against expected function return type
    if (curr_func_ret_type == ret_expr_type || curr_func_ret_type == T_KEY_UNIT){
        // all good
    }
    else if (curr_func_ret_type == T_KEY_DOUBLE && ret_expr_type == T_KEY_INT){
        ast_node->data->return_node.expr_or_call = ast_convert_int2float(ast_node->data->return_node.expr_or_call);
    }
    else{
        error_exit(SEM_TYPE_ERR, ast_node->line_num, "SEMANTIC ANALYSIS: Expected different data type to be returned");
    }

    return ret_expr_type;
}

static data_type sem_check_call(ast_node_t *ast_node, symtable_stack_t *stack) {
    symtable_bvs_node_t *func_symbol = symtable_stack_search(stack, ast_node->data->call_node.id_val);
    if (func_symbol == NULL || func_symbol->symbol_data.id_type != ID_FUNC){
        error_exit(SEM_UNDEF_ERR, ast_node->line_num, "SEMANTIC_ANALYSIS: Attempted to call undefined function");
    }

    if (func_symbol->symbol_data.data_type == T_NO_TYPE) {
        error_exit(SEM_TYPE_ERR, ast_node->line_num, "SEMANTIC ANALYSIS: Direct or indirect recursion requires explicit return type!");
    }

    ast_node_t *curr_arg = ast_node->data->call_node.first_arg;
    param_type_t *expected_param = func_symbol->symbol_data.func_info.param_types->first;  
    unsigned func_param_counter = 0;

    while(curr_arg != NULL && expected_param != NULL){

        ast_node_t *temp_next = curr_arg->next;
        curr_arg->next = NULL;
        data_type curr_arg_type = sem_anal(curr_arg, stack);
        curr_arg->next = temp_next;

        if (expected_param->type != T_ANY_TYPE && curr_arg_type != expected_param->type){
            if (expected_param->type == T_KEY_DOUBLE && curr_arg_type == T_KEY_INT) {
                curr_arg = ast_convert_int2float(curr_arg);
            } else {
                error_exit(SEM_ARG_ERR, ast_node->line_num, "SEMANTIC ANALYSIS: Argument types do not match!");
            }
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

static data_type sem_check_block(ast_node_t *ast_node, symtable_stack_t *stack) {
    symtable_stack_push(stack);
    data_type block_type = sem_anal(ast_node->data->block_node.first, stack);
    symtable_stack_pop(stack);
    return block_type;
}

static data_type sem_check_while(ast_node_t *ast_node, symtable_stack_t *stack) {
    data_type cond_type = sem_anal(ast_node->data->while_node.condition, stack);
    if (cond_type != T_KEY_BOOLEAN){
        error_exit(SEM_TYPE_ERR, ast_node->line_num, "SEMANTIC ANALYSIS: not returning boolean as a result of 'while' condition!");
    }
    sem_anal(ast_node->data->while_node.body, stack);
    return T_KEY_UNIT;
}

static data_type sem_check_if(ast_node_t *ast_node, symtable_stack_t *stack) {
    data_type cond_type = sem_anal(ast_node->data->if_node.condition, stack);
    if (cond_type != T_KEY_BOOLEAN){
        error_exit(SEM_TYPE_ERR, ast_node->line_num, "SEMANTIC ANALYSIS: not returning Boolean as a result of 'if' condition!");
    }
    
    sem_anal(ast_node->data->if_node.then_body, stack);
    
    if (ast_node->data->if_node.else_body != NULL){
        sem_anal(ast_node->data->if_node.else_body, stack);
    }
    
    return T_KEY_UNIT;
}

static data_type sem_check_declaration(ast_node_t *ast_node, symtable_stack_t *stack) {
    data_type expr_type = sem_anal(ast_node->data->declaration_node.expr_or_call, stack);
    data_type var_type = ast_node->data->declaration_node.data_type;

    if (strcmp(ast_node->data->declaration_node.id_val, "_") == 0){
        return T_KEY_UNIT;  //No symtable_insertion
    }  

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
    new_symbol.unique_id = unique_symbol_counter++;
    new_symbol.data_type = ast_node->data->declaration_node.data_type;
    new_symbol.id_type = ast_node->data->declaration_node.is_mutable ? ID_VAR : ID_VAL;
    
    // Redefinition check before insert
    if (symtable_find_symbol(stack->top->symtable, ast_node->data->declaration_node.id_val) != NULL){
        error_exit(SEM_REDEF_ERR, ast_node->line_num, "SEMANTIC ANALYSIS: Redefinition of a variable!");
    }
    stack->top->symtable = symtable_insert(stack->top->symtable, ast_node->data->declaration_node.id_val, new_symbol);
    ast_node->data->declaration_node.symbol_data = new_symbol;

    return T_KEY_UNIT;
}

static data_type sem_check_assign(ast_node_t *ast_node, symtable_stack_t *stack) {
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
        // OK
    }
    else if (symbol->symbol_data.data_type == T_KEY_DOUBLE && expr_or_call_type == T_KEY_INT){
        ast_node->data->assign_node.expr_or_call = ast_convert_int2float(ast_node->data->assign_node.expr_or_call);
    }
    else{
        error_exit(SEM_TYPE_ERR, ast_node->line_num, "SEMANTIC ANALYSIS: Incompatible types assignment");
    }

    ast_node->data->assign_node.symbol_data = symbol->symbol_data;

    return T_KEY_UNIT;
}

static data_type sem_check_operation(ast_node_t *ast_node, symtable_stack_t *stack) {
    data_type left_type = T_NO_TYPE;
    if (ast_node->data->op_node.left != NULL) {
        left_type = sem_anal(ast_node->data->op_node.left, stack);
    }
    data_type right_type = T_NO_TYPE;
    if (ast_node->data->op_node.right != NULL) {
        right_type = sem_anal(ast_node->data->op_node.right, stack);
    }
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

    if (op == EP_UNARY_MINUS || op == EP_UNARY_NEG) {
        
        data_type operand_type = (ast_node->data->op_node.left != NULL) ? left_type : right_type;
        
        if (op == EP_UNARY_MINUS) {
            if (operand_type == T_KEY_INT || operand_type == T_KEY_DOUBLE) {
                return operand_type;
            }
            error_exit(SEM_TYPE_ERR, ast_node->line_num, "SEMANTIC ANALYSIS: Invalid operand for unary minus");
        }
        else { 
            if (operand_type != T_KEY_BOOLEAN) {
                error_exit(SEM_TYPE_ERR, ast_node->line_num, "SEMANTIC ANALYSIS: ! requires a Boolean operand");
            }
            return T_KEY_BOOLEAN;
        }
    }

    if (left_type == T_NO_TYPE || right_type == T_NO_TYPE) {
        error_exit(SEM_TYPE_ERR, ast_node->line_num, "SEMANTIC ANALYSIS: Missing operand for binary operation");
    }

    // Arithmetics and concatenation
    if (op == T_PLUS || op == T_MINUS || op == T_MULT || op == T_DIV){
        if (left_type == T_KEY_STRING && right_type == T_KEY_STRING && op == T_PLUS) {
            return T_KEY_STRING; // String concatenation
        }
        if (left_type == right_type && (left_type == T_KEY_INT || left_type == T_KEY_DOUBLE)){
            return left_type;
        }
        error_exit(SEM_TYPE_ERR, ast_node->line_num, "SEMANTIC ANALYSIS: Invalid operands for arithmetics");
    }
    // Relation operators
    if (op == T_LT || op == T_LTE || op == T_GT || op == T_GTE || op == T_EQ || op == T_NEQ){
        if (left_type != right_type){
            error_exit(SEM_TYPE_ERR, ast_node->line_num, "SEMANTIC ANALYSIS: Invalid data types comparison in 'if' condition");
        }
        if (left_type == T_KEY_BOOLEAN && (op == T_LT || op == T_LTE || op == T_GT || op == T_GTE)){
            error_exit(SEM_TYPE_ERR, ast_node->line_num, "SEMANTIC ANALYSIS: Booleans can only be compared with '==' and '!='");
        }
        return T_KEY_BOOLEAN;
    }

    // Binary Logical operators
    if (op == T_AND || op == T_OR){
        if (left_type != T_KEY_BOOLEAN || right_type != T_KEY_BOOLEAN){
            error_exit(SEM_TYPE_ERR, ast_node->line_num, "SEMANTIC ANALYSIS: && and || require Boolean operands");
        }
        return T_KEY_BOOLEAN;
    }

    error_exit(SEM_OTHER_ERR, ast_node->line_num, "SEMANTIC ANALYSIS: Unknown operator in expression");
    return T_NO_TYPE; 
}

static data_type sem_check_literal(ast_node_t *ast_node, symtable_stack_t *stack) {
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
    else if (lit_type == T_BOOLEAN_LITERAL){
        return T_KEY_BOOLEAN;
    }
    else {
        error_exit(SEM_OTHER_ERR, ast_node->line_num, "SEMANTIC ANALYSIS: Unknown literal type");
    }
    return T_NO_TYPE;
}

static data_type sem_check_identifier(ast_node_t *ast_node, symtable_stack_t *stack) {
    symtable_bvs_node_t *symtable_node = symtable_stack_search(stack, ast_node->data->id_leaf_node.id_val);
    if(symtable_node == NULL){
        error_exit(SEM_UNDEF_ERR, ast_node->line_num, "SEMANTIC ANALYSIS: Using non existing identifier");
    }
    else{
        ast_node->data->id_leaf_node.symbol_data = symtable_node->symbol_data;
        return symtable_node->symbol_data.data_type;
    }
}


data_type sem_anal(ast_node_t *ast_node, symtable_stack_t *stack){
    data_type last_evaluated_type = T_KEY_UNIT;
    
    while (ast_node != NULL) {
        switch (ast_node->stmt_type){
            case AST_DEFINITION:
                last_evaluated_type = sem_check_definition(ast_node, stack);
                break;
            case AST_RETURN:
                last_evaluated_type = sem_check_return(ast_node, stack);
                break;
            case AST_CALL:
                last_evaluated_type = sem_check_call(ast_node, stack);
                break;
            case AST_BLOCK:
                last_evaluated_type = sem_check_block(ast_node, stack);
                break;
            case AST_WHILE:
                last_evaluated_type = sem_check_while(ast_node, stack);
                break;
            case AST_IF:
                last_evaluated_type = sem_check_if(ast_node, stack);
                break;
            case AST_DECLARATION:
                last_evaluated_type = sem_check_declaration(ast_node, stack);
                break;
            case AST_ASSIGN:
                last_evaluated_type = sem_check_assign(ast_node, stack);
                break;
            case EXPR_OPERATION:
                last_evaluated_type = sem_check_operation(ast_node, stack);
                break;
            case EXPR_LITERAL:
                last_evaluated_type = sem_check_literal(ast_node, stack);
                break;
            case EXPR_IDENTIFIER:
                last_evaluated_type = sem_check_identifier(ast_node, stack);
                break;
            default:
                break;
        }   
        ast_node = ast_node->next; // move to the next AST node
    }
    return last_evaluated_type;
}


//EXECUTE SEMANTIC ANALYSIS

void run_sem_anal(ast_node_t *ast_node) {
    unique_symbol_counter = 1; // Reset just for tests

    symtable_stack_t symtable_stack;
    symtable_stack_initialize(&symtable_stack);

    active_sem_stack = &symtable_stack;

    symtable_stack_push(&symtable_stack);

    builtin_functions_insertion(&symtable_stack);
    sem_pre_anal(ast_node, &symtable_stack);
    sem_anal(ast_node, &symtable_stack);
    
    symtable_stack_pop(&symtable_stack);
    
    active_sem_stack = NULL;
}

 
// CLEANUP FUNCTION FOR error_exit()

void sem_anal_error_exit(){
    if (active_sem_stack != NULL){
        while(!symtable_stack_is_empty(active_sem_stack)){
            symtable_stack_pop(active_sem_stack);
        }
        active_sem_stack = NULL;
    }
}
