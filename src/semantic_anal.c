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


#include "semantic_anal.h"

static data_type curr_func_ret_type;              // current function return type helper variable
static symtable_stack_t *active_sem_stack = NULL; // used for memory handling during error exit
static unsigned unique_symbol_counter = 1;        // represents unique id across all symtables

// BUILT-IN FUNCTIONS INSERTION INTO GLOBAL SYMTABLE

void builtin_functions_insertion(symtable_stack_t *stack)
{
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

void print_insertion(symtable_stack_t *stack)
{
    symbol_data_t symbol = {0};
    param_type_list_t params;
    param_type_list_initialize(&params);

    symbol.id_type = ID_FUNC;
    symbol.data_type = T_KEY_UNIT;
    symbol.func_info.is_defined = true;
    symbol.func_info.is_main = false;
    param_type_list_insert(&params, T_ANY_TYPE);
    symbol.func_info.param_types = params;

    stack->top->symtable = symtable_insert(stack->top->symtable, "print", symbol);
}

void readline_insertion(symtable_stack_t *stack)
{
    symbol_data_t symbol = {0};
    param_type_list_t params;
    param_type_list_initialize(&params);

    symbol.id_type = ID_FUNC;
    symbol.data_type = T_KEY_STRING;
    symbol.func_info.is_defined = true;
    symbol.func_info.is_main = false;
    symbol.func_info.param_types = params;

    stack->top->symtable = symtable_insert(stack->top->symtable, "readLine", symbol);
}

void readint_insertion(symtable_stack_t *stack)
{
    symbol_data_t symbol = {0};
    param_type_list_t params;
    param_type_list_initialize(&params);

    symbol.id_type = ID_FUNC;
    symbol.data_type = T_KEY_INT;
    symbol.func_info.is_defined = true;
    symbol.func_info.is_main = false;
    symbol.func_info.param_types = params;

    stack->top->symtable = symtable_insert(stack->top->symtable, "readInt", symbol);
}

void readdouble_insertion(symtable_stack_t *stack)
{
    symbol_data_t symbol = {0};
    param_type_list_t params;
    param_type_list_initialize(&params);

    symbol.id_type = ID_FUNC;
    symbol.data_type = T_KEY_DOUBLE;
    symbol.func_info.is_defined = true;
    symbol.func_info.is_main = false;
    symbol.func_info.param_types = params;

    stack->top->symtable = symtable_insert(stack->top->symtable, "readDouble", symbol);
}

void tostr_insertion(symtable_stack_t *stack)
{
    symbol_data_t symbol = {0};
    param_type_list_t params;
    param_type_list_initialize(&params);

    symbol.id_type = ID_FUNC;
    symbol.data_type = T_KEY_STRING;
    symbol.func_info.is_defined = true;
    symbol.func_info.is_main = false;
    param_type_list_insert(&params, T_ANY_TYPE);
    symbol.func_info.param_types = params;

    stack->top->symtable = symtable_insert(stack->top->symtable, "toStr", symbol);
}

void toint_insertion(symtable_stack_t *stack)
{
    symbol_data_t symbol = {0};
    param_type_list_t params;
    param_type_list_initialize(&params);

    symbol.id_type = ID_FUNC;
    symbol.data_type = T_KEY_INT;
    symbol.func_info.is_defined = true;
    symbol.func_info.is_main = false;
    param_type_list_insert(&params, T_KEY_DOUBLE);
    symbol.func_info.param_types = params;

    stack->top->symtable = symtable_insert(stack->top->symtable, "toInt", symbol);
}

void strlength_insertion(symtable_stack_t *stack)
{
    symbol_data_t symbol = {0};
    param_type_list_t params;
    param_type_list_initialize(&params);

    symbol.id_type = ID_FUNC;
    symbol.data_type = T_KEY_INT;
    symbol.func_info.is_defined = true;
    symbol.func_info.is_main = false;
    param_type_list_insert(&params, T_KEY_STRING);
    symbol.func_info.param_types = params;

    stack->top->symtable = symtable_insert(stack->top->symtable, "strLength", symbol);
}

void substr_insertion(symtable_stack_t *stack)
{
    symbol_data_t symbol = {0};
    param_type_list_t params;
    param_type_list_initialize(&params);

    symbol.id_type = ID_FUNC;
    symbol.data_type = T_KEY_STRING;
    symbol.func_info.is_defined = true;
    symbol.func_info.is_main = false;
    param_type_list_insert(&params, T_KEY_STRING);
    param_type_list_insert(&params, T_KEY_INT);
    param_type_list_insert(&params, T_KEY_INT);
    symbol.func_info.param_types = params;

    stack->top->symtable = symtable_insert(stack->top->symtable, "subStr", symbol);
}

void ord_insertion(symtable_stack_t *stack)
{
    symbol_data_t symbol = {0};
    param_type_list_t params;
    param_type_list_initialize(&params);

    symbol.id_type = ID_FUNC;
    symbol.data_type = T_KEY_INT;
    symbol.func_info.is_defined = true;
    symbol.func_info.is_main = false;
    param_type_list_insert(&params, T_KEY_STRING);
    param_type_list_insert(&params, T_KEY_INT);
    symbol.func_info.param_types = params;

    stack->top->symtable = symtable_insert(stack->top->symtable, "ord", symbol);
}

void chr_insertion(symtable_stack_t *stack)
{
    symbol_data_t symbol = {0};
    param_type_list_t params;
    param_type_list_initialize(&params);

    symbol.id_type = ID_FUNC;
    symbol.data_type = T_KEY_STRING;
    symbol.func_info.is_defined = true;
    symbol.func_info.is_main = false;
    param_type_list_insert(&params, T_KEY_INT);
    symbol.func_info.param_types = params;

    stack->top->symtable = symtable_insert(stack->top->symtable, "chr", symbol);
}

void readboolean_insertion(symtable_stack_t *stack)
{
    symbol_data_t symbol = {0};
    param_type_list_t params;
    param_type_list_initialize(&params);

    symbol.id_type = ID_FUNC;
    symbol.data_type = T_KEY_BOOLEAN;
    symbol.func_info.is_defined = true;
    symbol.func_info.is_main = false;
    symbol.func_info.param_types = params;

    stack->top->symtable = symtable_insert(stack->top->symtable, "readBoolean", symbol);
}


// FIRST GO THROUGH AST

void sem_pre_anal(ast_node_t *ast_node, symtable_stack_t *stack)
{

    bool main_found = false;
    while(ast_node != NULL && ast_node->stmt_type == AST_DEFINITION) {
        if(symtable_find_symbol(stack->top->symtable, ast_node->data->definition_node.id_val) != NULL) {
            error_exit(SEM_REDEF_ERR, ast_node->line_num, "Redefinition of a function is not allowed!");
        }
        if(strcmp(ast_node->data->definition_node.id_val, "_") == 0) {
            error_exit(SEM_OTHER_ERR, ast_node->line_num, "'_' cannot be used as a function name");
        }

        // Checks for @main
        if(ast_node->data->definition_node.is_main == true) {
            if(main_found == true) {
                error_exit(SEM_REDEF_ERR, ast_node->line_num, "Multiple @main functions are not allowed!");
            }
            main_found = true;

            if(ast_node->data->definition_node.first_param != NULL) {
                error_exit(SEM_ARG_ERR, ast_node->line_num, "There cannot be any arguments in @main function!");
            }
            if(ast_node->data->definition_node.data_type != T_KEY_UNIT) {
                error_exit(SEM_TYPE_ERR, ast_node->line_num, "@main function must return Unit!");
            }
        }

        // Prepare new function symbol for insertion into symtable
        symbol_data_t new_symbol = {0};
        new_symbol.data_type = ast_node->data->definition_node.data_type;
        new_symbol.id_type = ID_FUNC;
        param_type_list_initialize(&new_symbol.func_info.param_types); // expected parameter types list
        ast_node_t *parameter = ast_node->data->definition_node.first_param;
        while(parameter != NULL) {
            param_type_list_insert(&new_symbol.func_info.param_types, parameter->data->param_node.data_type);
            parameter = parameter->next;
        }
        new_symbol.func_info.is_defined = true;
        new_symbol.func_info.is_main = ast_node->data->definition_node.is_main;
        stack->top->symtable = symtable_insert(stack->top->symtable, ast_node->data->definition_node.id_val, new_symbol);

        ast_node = ast_node->next; // next function
    }
    if(main_found == false) {
        error_exit(SEM_UNDEF_ERR, 0, "Missing @main function!");
    }
}

/*
    This function was generated by Gemini 3.1 Pro and was modified with custom comments
    Prompt:
       "implement a function bool ends_with_return(ast_node_t *node),
       which returns true, if the last node (move with node->next) is return node,
       return false for other occurences. This must function recursively."
*/
static bool ends_with_return(ast_node_t *node)
{
    if(node == NULL) {
        return false;
    }
    // Get to the last node
    while(node->next != NULL) {
        node = node->next;
    }
    // Success
    if(node->stmt_type == AST_RETURN) {
        return true;
    }
    // We need to get the inside of the block
    if(node->stmt_type == AST_BLOCK) {
        return ends_with_return(node->data->block_node.first);
    }
    // Nested IF, Both branches need to guarantee return
    if(node->stmt_type == AST_IF) {
        if(node->data->if_node.else_body == NULL) {
            return false; // Missing else
        }
        return ends_with_return(node->data->if_node.then_body) && ends_with_return(node->data->if_node.else_body);
    }
    // everything else (assign, while, ...)
    return false;
}


// FUNCTIONS CALLED IN sem_anal() BASED ON stmt_type (AST_DEFINITON, DECLARATION, WHILE, ...)

static data_type sem_check_definition(ast_node_t *ast_node, symtable_stack_t *stack)
{
    symtable_stack_push(stack); // Local parameters must be visible during body check

    ast_node_t *parameter = ast_node->data->definition_node.first_param;
    while(parameter != NULL) {
        // Preparing parameter for getting inserted into symtable
        symbol_data_t new_symbol = {0};
        new_symbol.unique_id = unique_symbol_counter++;
        new_symbol.data_type = parameter->data->param_node.data_type;
        new_symbol.id_type = ID_VAL;

        if(symtable_find_symbol(stack->top->symtable, parameter->data->param_node.id_val) != NULL) {
            error_exit(SEM_REDEF_ERR, ast_node->line_num, "Duplicate parameter names!");
        }
        if(strcmp(parameter->data->param_node.id_val, "_") == 0) {
            error_exit(SEM_OTHER_ERR, ast_node->line_num, "'_' cannot be used as a parameter name");
        }
        stack->top->symtable = symtable_insert(stack->top->symtable, parameter->data->param_node.id_val, new_symbol);
        parameter->data->param_node.symbol_data = new_symbol;

        parameter = parameter->next;
    }

    curr_func_ret_type = ast_node->data->definition_node.data_type; // Current function type must be known during body check
    data_type body_type = sem_anal(ast_node->data->definition_node.body, stack); //     <-- Body entry

    // getting the last node of the function to later see whether it is 'AST_IF' and types control can be performed
    ast_node_t *last_node = ast_node->data->definition_node.body;
    if(last_node != NULL && last_node->stmt_type == AST_BLOCK /*not a single line function*/) {
        last_node = last_node->data->block_node.first;
        while(last_node->next != NULL) {
            last_node = last_node->next;
        }
    }

    if(curr_func_ret_type == T_KEY_UNIT) { // Explicit UNIT doesn't require following check
        // OK
    }
    else {
        // Check ending with IF
        if(last_node != NULL && last_node->stmt_type == AST_IF) {
            if(last_node->data->if_node.else_body != NULL && ends_with_return(last_node->data->if_node.then_body) &&
               ends_with_return(last_node->data->if_node.else_body)) {
                // OK
            }
            else if(curr_func_ret_type == T_NO_TYPE) { // Types must be deduced as UNIT
                // OK
            }
            else {
                error_exit(SEM_OTHER_ERR, ast_node->line_num, "Function cannot end with 'if', conditions not met");
            }
        }
    }
    // One line body functions
    if(ast_node->data->definition_node.body->stmt_type != AST_BLOCK) { // Can't be a block
        if(curr_func_ret_type == body_type || curr_func_ret_type == T_KEY_UNIT || curr_func_ret_type == T_NO_TYPE) {
            // OK
        }
        else if(curr_func_ret_type == T_KEY_DOUBLE && body_type == T_KEY_INT) {
            ast_node->data->definition_node.body = ast_convert_int2float(ast_node->data->definition_node.body);
        }
        else {
            error_exit(SEM_TYPE_ERR, ast_node->line_num, "Return type mismatch in single-line function body");
        }
    }

    symtable_stack_pop(stack);

    // missing funcion type deduction after body is examined
    if(curr_func_ret_type == T_NO_TYPE) {
        ast_node->data->definition_node.data_type = body_type; // update AST

        // update in symtable
        symtable_bvs_node_t *func_sym = symtable_find_symbol(stack->top->symtable, ast_node->data->definition_node.id_val);
        if(func_sym == NULL) {
            error_exit(SEM_OTHER_ERR, ast_node->line_num, "Cannot find a funcion id"); // shouldn't happen
        }
        func_sym->symbol_data.data_type = body_type; // deduction
    }

    return T_KEY_UNIT;
}

static data_type sem_check_return(ast_node_t *ast_node, symtable_stack_t *stack)
{
    data_type ret_expr_type;

    if(curr_func_ret_type == T_NO_TYPE) {
        error_exit(SEM_TYPE_ERR, ast_node->line_num, "'return' used in function without explicit return type!");
    }

    if(ast_node->data->return_node.expr_or_call == NULL) {
        ret_expr_type = T_KEY_UNIT;
    }
    else {
        ret_expr_type = sem_anal(ast_node->data->return_node.expr_or_call, stack);
    }
    // Compare against expected function return type
    if(curr_func_ret_type == ret_expr_type || curr_func_ret_type == T_KEY_UNIT) {
        // all good
    }
    else if(curr_func_ret_type == T_KEY_DOUBLE && ret_expr_type == T_KEY_INT) {
        ast_node->data->return_node.expr_or_call = ast_convert_int2float(ast_node->data->return_node.expr_or_call);
    }
    else {
        error_exit(SEM_TYPE_ERR, ast_node->line_num, "Expected different data type to be returned");
    }

    return ret_expr_type;
}

static data_type sem_check_call(ast_node_t *ast_node, symtable_stack_t *stack)
{
    symtable_bvs_node_t *func_symbol =
        symtable_stack_search(stack, ast_node->data->call_node.id_val); // Search in stack for funtion symbol(overshadowing)

    if(func_symbol == NULL || func_symbol->symbol_data.id_type != ID_FUNC) {
        error_exit(SEM_UNDEF_ERR, ast_node->line_num, "Attempted to call undefined function");
    }

    if(func_symbol->symbol_data.data_type == T_NO_TYPE) {
        error_exit(SEM_TYPE_ERR, ast_node->line_num, "Direct or indirect recursion requires explicit return type!");
    }

    ast_node_t *curr_arg = ast_node->data->call_node.first_arg;
    ast_node_t *prev_arg = NULL;
    param_type_t *expected_param = func_symbol->symbol_data.func_info.param_types.first;

    while(curr_arg != NULL && expected_param != NULL) { // Check types and count of all parameters

        ast_node_t *temp_next = curr_arg->next;
        curr_arg->next = NULL; // So that argument check doesn't go to the next node in sem_anal
        data_type curr_arg_type = sem_anal(curr_arg, stack);

        if(expected_param->type != T_ANY_TYPE && curr_arg_type != expected_param->type) {
            if(expected_param->type == T_KEY_DOUBLE && curr_arg_type == T_KEY_INT) {
                curr_arg = ast_convert_int2float(curr_arg); // There can be implicit conversion to double in func arguments
                // match new node back to AST
                if(prev_arg == NULL) {
                    ast_node->data->call_node.first_arg = curr_arg;
                }
                else {
                    prev_arg->next = curr_arg;
                }
            }
            else {
                curr_arg->next = temp_next;
                error_exit(SEM_ARG_ERR, ast_node->line_num, "Argument types do not match!");
            }
        }
        curr_arg->next = temp_next; // reestablish connection
        prev_arg = curr_arg;
        curr_arg = temp_next;
        expected_param = expected_param->next_ptr;
    }
    if(curr_arg != NULL || expected_param != NULL) { // Check if number of parameters matches
        error_exit(SEM_ARG_ERR, ast_node->line_num, "Number of parameters doesn't match!");
    }
    return func_symbol->symbol_data.data_type;
}

static data_type sem_check_block(ast_node_t *ast_node, symtable_stack_t *stack)
{
    symtable_stack_push(stack);
    data_type block_type = sem_anal(ast_node->data->block_node.first, stack);
    symtable_stack_pop(stack);
    return block_type;
}

static data_type sem_check_while(ast_node_t *ast_node, symtable_stack_t *stack)
{
    data_type cond_type = sem_anal(ast_node->data->while_node.condition, stack); // Check condition in 'while'
    if(cond_type != T_KEY_BOOLEAN) {
        error_exit(SEM_TYPE_ERR, ast_node->line_num, "not returning boolean as a result of 'while' condition!");
    }

    sem_anal(ast_node->data->while_node.body, stack); // Perform check in 'while' body

    return T_KEY_UNIT;
}

static data_type sem_check_if(ast_node_t *ast_node, symtable_stack_t *stack)
{
    data_type cond_type = sem_anal(ast_node->data->if_node.condition, stack); // Check condition in 'if'
    if(cond_type != T_KEY_BOOLEAN) {
        error_exit(SEM_TYPE_ERR, ast_node->line_num, "not returning Boolean as a result of 'if' condition!");
    }

    sem_anal(ast_node->data->if_node.then_body, stack);

    if(ast_node->data->if_node.else_body != NULL) { // else not mandatory
        sem_anal(ast_node->data->if_node.else_body, stack);
    }

    return T_KEY_UNIT;
}

static data_type sem_check_declaration(ast_node_t *ast_node, symtable_stack_t *stack)
{
    data_type expr_type =
        sem_anal(ast_node->data->declaration_node.expr_or_call, stack); // right side is mandatory in declaration
    data_type var_type = ast_node->data->declaration_node.data_type;

    if(strcmp(ast_node->data->declaration_node.id_val, "_") == 0) {
        error_exit(SEM_OTHER_ERR, ast_node->line_num, "'_' is not a valid identifier");
    }

    // Static type deduction
    if(var_type == T_NO_TYPE) {
        var_type = expr_type;
        ast_node->data->declaration_node.data_type = var_type;
    }

    // Node compatibility check in assignment
    if(var_type == expr_type) {
        // OK
    }
    else if(var_type == T_KEY_DOUBLE && expr_type == T_KEY_INT) {
        ast_node->data->declaration_node.expr_or_call = ast_convert_int2float(ast_node->data->declaration_node.expr_or_call);
    }
    else {
        error_exit(SEM_TYPE_ERR, ast_node->line_num, "Incompatible types assignment in declaration");
    }
    // Preparation for inserting into symtable
    symbol_data_t new_symbol = {0};
    new_symbol.unique_id = unique_symbol_counter++;
    new_symbol.data_type = ast_node->data->declaration_node.data_type;
    new_symbol.id_type = ast_node->data->declaration_node.is_mutable ? ID_VAR : ID_VAL;

    // Redefinition check before insert
    if(symtable_find_symbol(stack->top->symtable, ast_node->data->declaration_node.id_val) != NULL) {
        error_exit(SEM_REDEF_ERR, ast_node->line_num, "Redefinition of a variable!");
    }
    stack->top->symtable = symtable_insert(stack->top->symtable, ast_node->data->declaration_node.id_val, new_symbol);
    ast_node->data->declaration_node.symbol_data = new_symbol; // Update in AST

    return T_KEY_UNIT;
}

static data_type sem_check_assign(ast_node_t *ast_node, symtable_stack_t *stack)
{
    symtable_bvs_node_t *symbol = symtable_stack_search(stack, ast_node->data->assign_node.id_val); // search stack
    if(symbol == NULL) {
        error_exit(SEM_UNDEF_ERR, ast_node->line_num, "Non existing variable!");
    }
    if(symbol->symbol_data.id_type != ID_VAR) { // found symbol cannot be unchangeable or a function
        error_exit(SEM_VAR_ERR, ast_node->line_num, "Attempted to assign value to a non VAR symbol!");
    }
    // Compare data type against assigned expression or call
    data_type expr_or_call_type = sem_anal(ast_node->data->assign_node.expr_or_call, stack);
    if(symbol->symbol_data.data_type == expr_or_call_type) {
        // OK
    }
    else if(symbol->symbol_data.data_type == T_KEY_DOUBLE && expr_or_call_type == T_KEY_INT) {
        ast_node->data->assign_node.expr_or_call = ast_convert_int2float(ast_node->data->assign_node.expr_or_call);
    }
    else {
        error_exit(SEM_TYPE_ERR, ast_node->line_num, "Incompatible types assignment");
    }
    ast_node->data->assign_node.symbol_data = symbol->symbol_data; // update AST

    return T_KEY_UNIT;
}

static data_type sem_check_operation(ast_node_t *ast_node, symtable_stack_t *stack)
{
    data_type left_type = T_NO_TYPE;
    if(ast_node->data->op_node.left != NULL) {
        left_type = sem_anal(ast_node->data->op_node.left, stack);
    }
    data_type right_type = T_NO_TYPE;
    if(ast_node->data->op_node.right != NULL) {
        right_type = sem_anal(ast_node->data->op_node.right, stack);
    }
    token_type op = ast_node->data->op_node.op_type;

    // Implicit conversion INT --> DOUBLE
    if(left_type == T_KEY_INT && right_type == T_KEY_DOUBLE) {
        ast_node->data->op_node.left = ast_convert_int2float(ast_node->data->op_node.left);
        left_type = T_KEY_DOUBLE;
    }
    else if(left_type == T_KEY_DOUBLE && right_type == T_KEY_INT) {
        ast_node->data->op_node.right = ast_convert_int2float(ast_node->data->op_node.right);
        right_type = T_KEY_DOUBLE;
    }
    // Unary operators
    if(op == EP_UNARY_MINUS || op == EP_UNARY_NEG) {
        data_type operand_type = (ast_node->data->op_node.left != NULL) ? left_type : right_type; // either left type or right
                                                                                                  // type

        if(op == EP_UNARY_MINUS) {
            if(operand_type == T_KEY_INT || operand_type == T_KEY_DOUBLE) { // Unary minus works for numeric types
                return operand_type;
            }
            error_exit(SEM_TYPE_ERR, ast_node->line_num, "Invalid operand for unary minus");
        }
        else {
            if(operand_type != T_KEY_BOOLEAN) {
                error_exit(SEM_TYPE_ERR, ast_node->line_num, "! requires a Boolean operand");
            }
            return T_KEY_BOOLEAN;
        }
    }
    // Check before binary operations but this shouldn't even happen
    if(left_type == T_NO_TYPE || right_type == T_NO_TYPE) {
        error_exit(SEM_OTHER_ERR, ast_node->line_num, "Missing operand for binary operation");
    }

    // Arithmetics and concatenation
    if(op == T_PLUS || op == T_MINUS || op == T_MULT || op == T_DIV) {
        if(left_type == T_KEY_STRING && right_type == T_KEY_STRING && op == T_PLUS) {
            return T_KEY_STRING; // String concatenation
        }
        if(left_type == right_type && (left_type == T_KEY_INT || left_type == T_KEY_DOUBLE)) {
            return left_type;
        }
        error_exit(SEM_TYPE_ERR, ast_node->line_num, "Invalid operands for arithmetics");
    }
    // Relation operators
    if(op == T_LT || op == T_LTE || op == T_GT || op == T_GTE || op == T_EQ || op == T_NEQ) {
        if(left_type != right_type) {
            error_exit(SEM_TYPE_ERR, ast_node->line_num, "Invalid data types comparison in 'if' condition");
        }
        if(op == T_LT || op == T_LTE || op == T_GT || op == T_GTE) {
            if(left_type != T_KEY_INT && left_type != T_KEY_DOUBLE && left_type != T_KEY_STRING) {
                error_exit(SEM_TYPE_ERR, ast_node->line_num, "<, <=, >, >= can only compare numbers or strings");
            }
        }
        return T_KEY_BOOLEAN;
    }

    // Binary Logical operators
    if(op == T_AND || op == T_OR) {
        if(left_type != T_KEY_BOOLEAN || right_type != T_KEY_BOOLEAN) {
            error_exit(SEM_TYPE_ERR, ast_node->line_num, "&& and || require Boolean operands");
        }
        return T_KEY_BOOLEAN;
    }

    error_exit(SEM_OTHER_ERR, ast_node->line_num, "Unknown operator in expression"); // to be sure, shouldn't happen

    return T_NO_TYPE;
}

static data_type sem_check_literal(ast_node_t *ast_node, symtable_stack_t *stack)
{ // literal inside of expression check
    int lit_type = ast_node->data->lit_leaf_node.lit_type;
    if(lit_type == T_DOUBLE_LITERAL) {
        return T_KEY_DOUBLE;
    }
    else if(lit_type == T_INT_LITERAL) {
        return T_KEY_INT;
    }
    else if(lit_type == T_STRING_LITERAL) {
        return T_KEY_STRING;
    }
    else if(lit_type == T_UNIT_LITERAL) {
        return T_KEY_UNIT;
    }
    else if(lit_type == T_BOOLEAN_LITERAL) {
        return T_KEY_BOOLEAN;
    }
    else {
        error_exit(SEM_OTHER_ERR, ast_node->line_num, "Unknown literal type");
    }

    return T_NO_TYPE;
}

static data_type sem_check_identifier(ast_node_t *ast_node, symtable_stack_t *stack)
{ // identifier inside of expression check
    symtable_bvs_node_t *symtable_node = symtable_stack_search(stack, ast_node->data->id_leaf_node.id_val);
    if(symtable_node == NULL) {
        error_exit(SEM_UNDEF_ERR, ast_node->line_num, "Using non existing identifier");
    }
    if(symtable_node->symbol_data.id_type == ID_FUNC) {
        error_exit(SEM_TYPE_ERR, ast_node->line_num, "Function cannot be used as a variable in an expression");
    }
    ast_node->data->id_leaf_node.symbol_data = symtable_node->symbol_data; // update AST

    return symtable_node->symbol_data.data_type;
}


// SECOND GO THROUGH AST

data_type sem_anal(ast_node_t *ast_node, symtable_stack_t *stack)
{
    data_type last_evaluated_type = T_KEY_UNIT; // Tracks the type of the last evaluated node

    while(ast_node != NULL) {
        switch(ast_node->stmt_type) {
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
        ast_node = ast_node->next;
    }
    return last_evaluated_type;
}


// EXECUTE SEMANTIC ANALYSIS

void run_sem_anal(ast_node_t *ast_node)
{
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

void sem_anal_error_exit()
{
    if(active_sem_stack != NULL) {
        while(!symtable_stack_is_empty(active_sem_stack)) {
            symtable_stack_pop(active_sem_stack); // Clean each symtable
        }
        active_sem_stack = NULL;
    }
}
