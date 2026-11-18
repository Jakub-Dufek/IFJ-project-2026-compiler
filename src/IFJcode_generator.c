/**
 * @file IFJcode_generator.c
 * @internal Part of IFJ-project-2026-compiler
 *
 * @brief FSM for code generator
 * @author Šimon Greger (xgreges00)
 * @author Samuel Vít Lehotský (xlehots00)
 * @date 25.10.2026
 *
 * @details This file contains finite state machine representing the functions of a IFJcode26 generator
 */

#include "IFJcode_generator.h"
#include "ast.h"
#include <stdio.h>
#include <string.h>


void print_string_literal(char *string, bool flag) {
    printf("string@");
    for (unsigned idx = 0; string[idx] != '\0'; idx++) {
        if (string[idx] <= 32 || string[idx] == '#' || string[idx] == '\\') {
            printf("\\%03u", (unsigned)string[idx]);
        }
        else {
            putchar(string[idx]);
        }
    }
    if (flag) {
        putchar('\n');
    }
}

token_type find_operand_type (ast_node_t *node) {
    token_type type = T_UNDEFINED;
    switch (node->stmt_type)
    {
    case EXPR_LITERAL:
        switch (node->data->lit_leaf_node.lit_type)
        {
        case T_INT_LITERAL:
            type = T_KEY_INT;
            break;

        case T_DOUBLE_LITERAL:
            type = T_KEY_DOUBLE;
            break;

        case T_STRING_LITERAL:
            type = T_KEY_STRING;
            break;

        case T_BOOLEAN_LITERAL:
            type = T_KEY_BOOLEAN;
            break;
        
        default:
            type = T_UNDEFINED;
            break;
        }
        break;
    
    case EXPR_IDENTIFIER:
        type = node->data->id_leaf_node.symbol_data.data_type;
        break;

    case EXPR_INT2FLOAT:
        type = T_KEY_DOUBLE;
        break;
    
    case EXPR_OPERATION: {
        // Handle unary operations seperatelly
        if (node->data->op_node.op_type == EP_UNARY_MINUS) {
            return find_operand_type(node->data->op_node.right);
        }
        else if (node->data->op_node.op_type == EP_UNARY_NEG) {
            return T_KEY_BOOLEAN;
        }

        if (node->data->op_node.op_type == T_EQ || node->data->op_node.op_type == T_NEQ || 
            node->data->op_node.op_type == T_GT || node->data->op_node.op_type == T_GTE ||
            node->data->op_node.op_type == T_LT || node->data->op_node.op_type == T_LTE) {
            return T_KEY_BOOLEAN;
        }

        token_type type_left = find_operand_type(node->data->op_node.left);
        token_type type_right = find_operand_type(node->data->op_node.right);
        if (type_left == T_KEY_INT && type_right == T_KEY_INT) {
            type = T_KEY_INT;
        }
        else if (type_left == T_KEY_DOUBLE || type_right == T_KEY_DOUBLE) {
            type = T_KEY_DOUBLE;
        }
        else if (type_left == T_KEY_STRING && type_right == T_KEY_STRING) {
            type = T_KEY_STRING;
        }
        break;
    }
    default:
        break;
    }

    return type;
}

void generate_if(ast_node_t *node, context_t *context) {
    unsigned label_num = context->label_idx++;
    ast_if_node_t if_node = node->data->if_node;

    // Process the condition
    process_node(if_node.condition, context);
    // Push true for JUMP to compare to
    printf("PUSHS bool@true\n");
    if (if_node.else_body != NULL) {
        printf("JUMPIFNEQS $else%u\n", label_num);
    }
    else {
        printf("JUMPIFNEQS $fi%u\n", label_num);
    }

    // Process 'then' block
    process_node(if_node.then_body, context);

    if (if_node.else_body != NULL) {
        printf("JUMP $fi%u\n", label_num);
        printf("LABEL $else%u\n", label_num);
        process_node(if_node.else_body, context);
    }

    // End of this if
    printf("LABEL $fi%u\n", label_num);
}

void generate_while(ast_node_t *node, context_t *context) {
    unsigned label_num = context->label_idx++;
    ast_while_node_t while_node = node->data->while_node;

    printf("LABEL $while%u\n", label_num);

    // Process the condition
    process_node(while_node.condition, context);
    printf("PUSHS bool@true\n");
    printf("JUMPIFNEQS $elihw%u\n", label_num);

    process_node(while_node.body, context);
    printf("JUMP $while%u\n", label_num);

    printf("LABEL $elihw%u\n", label_num);
}

void generate_block(ast_node_t *node, context_t *context, bool result_needed) {
    ast_node_t *block_node = node->data->block_node.first;
    while (block_node != NULL) {
        process_node(block_node, context);
        // Throw away any expressions that are not needed, but only if they are not the last instruction in the block 
        // and its result is needed by the caller
        if (is_expression(block_node, context) == true && (block_node->next != NULL || result_needed == false)) {
            printf("POPS LF@discard_top_stack\n");
        }

        block_node = block_node->next;
    }
}

void generate_declaration(ast_node_t *node, char *frame, context_t *context) {
    ast_declaration_node_t dec_node = node->data->declaration_node;

    if (dec_node.expr_or_call == NULL) { 
        return; 
    }
    else {
        process_node(dec_node.expr_or_call, context);
        printf("POPS %s@var_%u\n", frame, dec_node.symbol_data.unique_id);
    }
}

void generate_assignment(ast_node_t *node, context_t *context) {
    process_node(node->data->assign_node.expr_or_call, context);
    printf("POPS LF@var_%u\n", node->data->assign_node.symbol_data.unique_id);
}

void generate_definitions_in_function(ast_node_t *node) {
    if (node == NULL) {
        return;
    }

    switch (node->stmt_type) {
        case AST_BLOCK:
            for (ast_node_t *stmt = node->data->block_node.first;
                 stmt != NULL;
                 stmt = stmt->next) {
                generate_definitions_in_function(stmt);
            }
            break;

        case AST_DECLARATION:
            printf("DEFVAR LF@var_%u\n", node->data->declaration_node.symbol_data.unique_id);
            break;

        case AST_IF:
            generate_definitions_in_function(node->data->if_node.then_body);
            generate_definitions_in_function(node->data->if_node.else_body);
            break;

        case AST_WHILE:
            generate_definitions_in_function(node->data->while_node.body);
            break;

        default:
            break;
    }
}

void generate_function(ast_node_t *node, context_t *context) {
    ast_definition_node_t def_node = node->data->definition_node;
    context->function_name = def_node.id_val;
    context->return_type = def_node.data_type;
    bool result_needed = false;
    if (def_node.data_type == T_KEY_INT || def_node.data_type == T_KEY_DOUBLE || 
        def_node.data_type == T_KEY_STRING || def_node.data_type == T_KEY_BOOLEAN) {
        result_needed = true;
    }

    // Create label and frame
    printf("LABEL $%s\n", def_node.id_val);
    printf("PUSHFRAME\n");

    // Prepare and load parameters
    int param_count = 0;
    for (ast_node_t *param = def_node.first_param; param != NULL; param = param->next) {
        printf("DEFVAR LF@var_%u\n", param->data->param_node.symbol_data.unique_id);
        printf("PUSHS LF@arg%i\n", param_count);
        printf("POPS LF@var_%u\n", param->data->param_node.symbol_data.unique_id);
        param_count++;
    }

    // Define all local variables at the start of the function
    printf("DEFVAR LF@discard_top_stack\n");
    generate_definitions_in_function(def_node.body);

    // Handle body
    if (def_node.body->stmt_type == AST_BLOCK) {
        generate_block(def_node.body, context, result_needed);
    }
    else {
        if (def_node.body->stmt_type == AST_RETURN) {
            generate_return(def_node.body, context);
        }
        else {
            process_node(def_node.body, context);
            if (result_needed == false && is_expression(def_node.body, context) == true) {
                printf("POPS LF@discard_top_stack\n");
            }
        }
    }


    /* Generated by ChatGPT
       Prompt: Jak můžu elegantně vyřešit "return" v rámci zanořených podmínek uvnitř funkce?
    */
    printf("LABEL $%s_end\n", def_node.id_val);

    if (def_node.data_type == T_KEY_UNIT) {
        printf("PUSHS nil@nil\n");
    }

    printf("POPFRAME\n");
    printf("RETURN\n");

    // Remove information from context
    context->function_name = NULL;
    context->return_type = T_UNDEFINED;
}

void generate_return(ast_node_t *node, context_t *context) {
    // If the function type is not Unit, process the return expression
    ast_node_t *last_instruction = node->data->return_node.expr_or_call;

    if (last_instruction != NULL) {
        process_node(last_instruction, context);
    }

    if (context->return_type == T_KEY_UNIT && is_expression(last_instruction, context) == true) {
        printf("POPS LF@discard_top_stack\n");
    }

    printf("JUMP $%s_end\n", context->function_name);
}

bool is_expression (ast_node_t *node, context_t *context) {
    if (node == NULL) {
        return false;
    }
    else if (node->stmt_type == EXPR_OPERATION || node->stmt_type == EXPR_LITERAL || 
        node->stmt_type == EXPR_IDENTIFIER || node->stmt_type == EXPR_INT2FLOAT || 
        node->stmt_type == AST_CALL && function_return_type(node->data->call_node.id_val, context) != T_UNDEFINED) {
        return true;
    }
    
    return false;
}

token_type function_return_type(char *id_val, context_t *context) {
    for (ast_node_t *node = context->root; node != NULL; node = node->next) {
        if (node->stmt_type == AST_DEFINITION && strcmp(node->data->definition_node.id_val, id_val) == 0) {
            return node->data->definition_node.data_type;
        }
    }
    return T_UNDEFINED;
}

void generate_call(ast_node_t *node, context_t *context) {
    // Check if the called function is not a built-in function
    if (generate_builtin_call(node, context) == false) {
        ast_call_node_t call_node = node->data->call_node;
        int arg_count = 0;

        for (ast_node_t *arg = call_node.first_arg; arg != NULL; arg = arg->next) {
            process_node(arg, context);
            arg_count++;
        }

        printf("CREATEFRAME\n");
        for (int counter = arg_count - 1; counter >= 0; counter--) {
            printf("DEFVAR TF@arg%i\n", counter);
            printf("POPS TF@arg%i\n", counter);
        }

        printf("CALL $%s\n", call_node.id_val);
    }
}

void generate_operation(ast_node_t *node, context_t *context) {
    // Special handling for unary minus, because all other operations are binary
    if (node->data->op_node.op_type == EP_UNARY_MINUS) {
        // Push a zero on the stack
        if (find_operand_type(node) == T_KEY_INT) {
            printf("PUSHS int@0\n");
        }
        else {
            printf("PUSHS float@0x0p+0\n");
        }

        process_node(node->data->op_node.right, context);
        printf("SUBS\n");
        return;
    } 
    else if (node->data->op_node.op_type == EP_UNARY_NEG) {
        process_node(node->data->op_node.right, context);
        printf("NOTS\n");
        return;
    }

    process_node(node->data->op_node.left, context);
    process_node(node->data->op_node.right, context);

    //Print the type of the operation
    switch (node->data->op_node.op_type)
    {
    case T_PLUS:
        if (find_operand_type(node->data->op_node.left) == T_KEY_STRING &&
            find_operand_type(node->data->op_node.right) == T_KEY_STRING) {
            printf("DEFVAR LF@left_%u", ++context->label_idx);
            printf("DEFVAR LF@right_%u", context->label_idx);
            printf("POPS LF@right_%u", context->label_idx);
            printf("POPS LF@left_%u", context->label_idx);
            printf("DEFVAR LF@result_%u", context->label_idx);
            printf("CONCAT LF@result_%u LF@left_%u LF@right_%u", context->label_idx, context->label_idx, context->label_idx);
            printf("PUSHS LF@result_%u", context->label_idx++);
        }
        else { printf("ADDS\n"); }
        // #TODO dodělat konkatenaci, můžu použít find_operand_type()
        break;

    case T_MINUS:
        printf("SUBS\n");
        break;

    case T_DIV:
        //Need to discover if I should use integer type or double type division
        token_type div_type_left = find_operand_type(node->data->op_node.left);
        token_type div_type_right = find_operand_type(node->data->op_node.right);
        if (div_type_left == T_KEY_INT && div_type_right == T_KEY_INT) {
            printf("IDIVS\n");
        }
        else if (div_type_left == T_KEY_DOUBLE || div_type_right == T_KEY_DOUBLE) {
            printf("DIVS\n");
        }
        else {
            // #TODO chyba, nejde dělit nic jiného než INT nebo DOUBLE
        }
        break;

    case T_MULT:
        printf("MULS\n");
        break;
        
    case T_GT:
        printf("GTS\n");
        break;

    case T_LT:
        printf("LTS\n");
        break;

    case T_EQ:
        printf("EQS\n");
        break;

    //Compare operations with equal options are just negations of non-equal counter parts
    case T_GTE:
        printf("LTS\n");
        printf("NOTS\n");
        break;

    case T_LTE:
        printf("GTS\n");
        printf("NOTS\n");
        break;

    case T_NEQ:
        printf("EQS\n");
        printf("NOTS\n");
        break;
    
    default:
        // #TODO chyba, neznama operace
        break;
    }
}

void generate_identifier(ast_node_t *node, context_t *context) {
    printf("PUSHS LF@var_%u\n", node->data->id_leaf_node.symbol_data.unique_id);
}

void generate_literal(ast_node_t *node, context_t *context) {
    printf("PUSHS ");

    switch (node->data->lit_leaf_node.lit_type)
    {
    case T_STRING_LITERAL:
        print_string_literal(node->data->lit_leaf_node.lit_value.string_value, true);
        //printf("string@%s\n", node->data->lit_leaf_node.lit_value.string_value);
        break;
    
    case T_INT_LITERAL:
        printf("int@%i\n", node->data->lit_leaf_node.lit_value.int_value);
        break;

    case T_DOUBLE_LITERAL:
        printf("float@%a\n", node->data->lit_leaf_node.lit_value.double_value);
        break;

    case T_UNIT_LITERAL:
        printf("nil@nil\n");
        break;

    case T_BOOLEAN_LITERAL:
        if (node->data->lit_leaf_node.lit_value.int_value == 1) {
            printf("bool@true\n");
        }
        else {
            printf("bool@false\n");
        }
        break;

    default:
        // #TODO error, nerozpoznaný typ literálu
        break;
    }
}

void process_node(ast_node_t *node, context_t *context) {
    switch (node->stmt_type)
    {
    case AST_BLOCK: {
        generate_block(node, context, false);
        break;
    }

    case AST_DECLARATION:
        generate_declaration(node, "LF", context);   // TODO mozna neni vzdy LF
        break;

    case AST_DEFINITION:
       generate_function(node, context);
        break;

    case EXPR_OPERATION:
        generate_operation(node, context);
        break;

    case EXPR_IDENTIFIER:
        generate_identifier(node, context);
        break;

    case EXPR_LITERAL:
        generate_literal(node, context);
        break;

    case EXPR_INT2FLOAT:
        process_node(node->data->imp_conversion.operand, context);
        printf("INT2FLOATS\n");
        break;

    case AST_ASSIGN:
        generate_assignment(node, context);
        break;

    case AST_CALL:
        generate_call(node, context);
        break;

    case AST_RETURN:
        generate_return(node, context);
        break;
    
    case AST_IF:
        generate_if(node, context);
        break;

    case AST_WHILE:
        generate_while(node, context);
        break;
    default:
        // #TODO error, nerozpoznaný typ uzlu
        break;
    }
}

void generate_print(ast_node_t *node) {
    switch(node->stmt_type) {
        case EXPR_LITERAL:
            switch(node->data->lit_leaf_node.lit_type) {
                case T_STRING_LITERAL:
                    printf("WRITE ");
                    print_string_literal(node->data->lit_leaf_node.lit_value.string_value, true);
                    break;

                case T_INT_LITERAL:
                    printf("WRITE int@%d\n", node->data->lit_leaf_node.lit_value.int_value);
                    break;

                case T_DOUBLE_LITERAL:
                    printf("WRITE float@%a\n", node->data->lit_leaf_node.lit_value.double_value);
                    break;

                case T_BOOLEAN_LITERAL:
                    if (node->data->lit_leaf_node.lit_value.int_value == 1) {
                        printf("WRITE bool@true\n");
                    }
                    else {
                        printf("WRITE bool@false\n");
                    }
                    break;

                case T_UNIT_LITERAL:
                    printf("WRITE string@()\n");
                    break;

                default:
                    //TODO ERROR
                    break;
            }
            break;

        case EXPR_IDENTIFIER:
            if (node->data->id_leaf_node.symbol_data.data_type == T_KEY_UNIT) {
                printf("WRITE string@()\n");
            }
            else {
                printf("WRITE LF@var_%u\n", node->data->id_leaf_node.symbol_data.unique_id);
            }
            break;

        default:
            //TODO ERRO
            break;
    }
}

void generate_readLine(char *target_var) {
    printf("READ %s string\n", target_var);
}

void generate_readInt(char *target_var, context_t *context) {
    printf("READ %s int\n", target_var);
    printf("JUMPIFNEQ $readInt_%u %s nil@nil\n", ++context->label_idx, target_var);
    printf("MOVE %s int@0\n", target_var);
    printf("LABEL $readInt_%u\n", context->label_idx++);
}

void generate_readDouble(char *target_var, context_t *context) {
    printf("READ %s float\n", target_var);
    printf("JUMPIFNEQ $readDouble_%u %s nil@nil\n", ++context->label_idx, target_var);
    printf("MOVE %s float@0x0p+0\n", target_var);
    printf("LABEL $readDouble_%u\n", context->label_idx++);
}

void generate_readBoolean(char *target_var) {
    printf("READ %s bool\n", target_var);
}

void generate_toStr(char *target_var, ast_node_t *node) {
    switch (node->stmt_type)
    {
    case EXPR_LITERAL:
        switch (node->data->lit_leaf_node.lit_type) {
            case T_STRING_LITERAL:
                printf("MOVE %s ", target_var);
                print_string_literal(node->data->lit_leaf_node.lit_value.string_value, true);
                break;

            case T_INT_LITERAL:
                printf("INT2STR %s int@%d\n", target_var, node->data->lit_leaf_node.lit_value.int_value);
                break;

            case T_DOUBLE_LITERAL:
                printf("FLOAT2STR %s float@%a\n", target_var, node->data->lit_leaf_node.lit_value.double_value);
                break;

            case T_UNIT_LITERAL:
                printf("MOVE %s string@()\n", target_var);
                break;
            
            default:
                //TODO ERROR
        }
        break;

    case EXPR_IDENTIFIER:
        unsigned id = node->data->id_leaf_node.symbol_data.unique_id;
        switch (node->data->id_leaf_node.symbol_data.data_type) {
            case T_KEY_STRING:
                printf("MOVE %s LF@var_%u\n", target_var, id);
                break;

            case T_KEY_INT:
                printf("INT2STR %s LF@var_%u\n", target_var, id);
                break;

            case T_KEY_DOUBLE:
                printf("FLOAT2STR %s LF@var_%u\n", target_var, id);
                break;

            case T_KEY_UNIT:
                printf("MOVE %s string@()\n", target_var);
                break;
            
            default:
                //TODO ERROR
        }
        break;
    
    default:
        //TODO ERROR
        break;
    }
}

void generate_toInt(char *target_var, ast_node_t *node) {
    switch (node->stmt_type)
    {
    case EXPR_LITERAL:
        if(node->data->lit_leaf_node.lit_type == T_DOUBLE_LITERAL) {
            printf("FLOAT2INT %s float@%a\n", target_var, node->data->lit_leaf_node.lit_value.double_value);
        }
        else {/*TODO ERROR*/}
        break;

    case EXPR_IDENTIFIER:
        if (node->data->id_leaf_node.symbol_data.data_type == T_KEY_DOUBLE) {
            printf("FLOAT2INT %s LF@var_%u\n", target_var, node->data->id_leaf_node.symbol_data.unique_id);
        }
        break;
    
    default:
        //TODO ERROR
        break;
    }
}

void generate_strLength(char *target_var, ast_node_t *node) {
    switch (node->stmt_type)
    {
    case EXPR_LITERAL:
        if(node->data->lit_leaf_node.lit_type == T_STRING_LITERAL) {
            printf("STRLEN %s ", target_var);
            print_string_literal(node->data->lit_leaf_node.lit_value.string_value, true);
        }
        else {/*TODO ERROR*/}
        break;

    case EXPR_IDENTIFIER:
        if (node->data->id_leaf_node.symbol_data.data_type == T_KEY_STRING) {
            printf("STRLEN %s LF@var_%u\n", target_var, node->data->id_leaf_node.symbol_data.unique_id);
        }
        break;
    
    default:
        //TODO ERROR
        break;
    }
}

void generate_subStr(ast_node_t *arg1, ast_node_t *arg2, ast_node_t *arg3) {
    printf("CREATEFRAME\n");
    printf("DEFVAR TF@str\n");
    printf("DEFVAR TF@idx\n");
    printf("DEFVAR TF@end\n");

    if (arg1->stmt_type == EXPR_LITERAL) {
        printf("MOVE TF@str ");
        print_string_literal(arg1->data->lit_leaf_node.lit_value.string_value, true);
    }
    else if (arg1->stmt_type == EXPR_IDENTIFIER) {
        printf("MOVE TF@str LF@var_%u\n", arg1->data->id_leaf_node.symbol_data.unique_id);
    }   

    if (arg2->stmt_type == EXPR_LITERAL) {
        printf("MOVE TF@idx int@%d\n", arg2->data->lit_leaf_node.lit_value.int_value);
    }
    else if (arg2->stmt_type == EXPR_IDENTIFIER) {
        printf("MOVE TF@idx LF@var_%u\n", arg2->data->id_leaf_node.symbol_data.unique_id);
    }

    if (arg3->stmt_type == EXPR_LITERAL) {
        printf("MOVE TF@end int@%d\n", arg3->data->lit_leaf_node.lit_value.int_value);
    }
    else if (arg3->stmt_type == EXPR_IDENTIFIER) {
        printf("MOVE TF@end LF@var_%u\n", arg3->data->id_leaf_node.symbol_data.unique_id);
    }

    printf("CALL $builtin_subStr_begin\n");
    printf("PUSHS TF@$return\n");
}

void generate_builtin_subStr() {
    printf("\nLABEL $builtin_subStr_begin\n");
    printf("PUSHFRAME\n");

    printf("DEFVAR LF@len\n");
    printf("DEFVAR LF@bool\n");
    printf("DEFVAR LF@char\n");
    printf("DEFVAR LF@$return\n");

    //value: str
    printf("STRLEN LF@len LF@str\n");
    printf("MOVE LF@$return string@\n");

    //start<0 start: idx
    printf("LT LF@bool LF@idx int@0\n");
    printf("JUMPIFEQ $subStr_error LF@bool bool@true\n");
    //end<0 end: end
    printf("LT LF@bool LF@end int@0\n");
    printf("JUMPIFEQ $subStr_error LF@bool bool@true\n");
    //start>end
    printf("GT LF@bool LF@idx LF@end\n");
    printf("JUMPIFEQ $subStr_error LF@bool bool@true\n");
    //start>=strLength(value)
    printf("LT LF@bool LF@idx LF@len\n");
    printf("NOT LF@bool LF@bool\n");
    printf("JUMPIFEQ $subStr_error LF@bool bool@true\n");
    //end>strLength(value)
    printf("GT LF@bool LF@end LF@len\n");
    printf("JUMPIFEQ $subStr_error LF@bool bool@true\n");
    printf("JUMP $subStr_loop\n");
    
    printf("LABEL $subStr_error\n");
    printf("EXIT int@40\n");

    printf("LABEL $subStr_loop\n");
    printf("JUMPIFEQ $subStr_end LF@idx LF@end\n");
    printf("GETCHAR LF@char LF@str LF@idx\n");
    printf("CONCAT LF@$return LF@$return LF@char\n");
    printf("ADD LF@idx LF@idx int@1\n");
    printf("JUMP $subStr_loop\n");
    
    printf("LABEL $subStr_end\n");
    printf("POPFRAME\n");
    printf("RETURN\n");
}

void generate_ord(char *target_var, ast_node_t *arg1, ast_node_t *arg2) {
    printf("STRI2INT %s  ", target_var);

    if (arg1->stmt_type == EXPR_LITERAL) {
        print_string_literal(arg1->data->lit_leaf_node.lit_value.string_value, false);
        printf(" ");
    }
    else if (arg1->stmt_type == EXPR_IDENTIFIER) {
        printf("LF@var_%u ", arg1->data->id_leaf_node.symbol_data.unique_id);
    }

    if (arg2->stmt_type == EXPR_LITERAL) {
        printf("int@%d\n", arg2->data->lit_leaf_node.lit_value.int_value);
    }
    else if (arg2->stmt_type == EXPR_IDENTIFIER) {
        printf("LF@var_%u\n", arg2->data->id_leaf_node.symbol_data.unique_id);
    }
}

void generate_chr(char *target_var, ast_node_t *node) {
    switch (node->stmt_type)
    {
    case EXPR_LITERAL:
        if(node->data->lit_leaf_node.lit_type == T_INT_LITERAL) {
            printf("INT2CHAR %s int@%d\n", target_var, node->data->lit_leaf_node.lit_value.int_value);
        }
        else {/*TODO ERROR*/}
        break;

    case EXPR_IDENTIFIER:
        if (node->data->id_leaf_node.symbol_data.data_type == T_KEY_INT) {
            printf("INT2CHAR %s LF@var_%u\n", target_var, node->data->id_leaf_node.symbol_data.unique_id);
        }
        break;
    
    default:
        //TODO ERROR
        break;
    }
}

char *find_main_function(ast_node_t *root) {
    for (ast_node_t *node = root; node != NULL; node = node->next) {
        if (node->stmt_type == AST_DEFINITION && node->data->definition_node.is_main == true) {
            return node->data->definition_node.id_val;
        }
    }

    // TODO chyba, nebyl nalezen main??
    return NULL;
}

bool generate_builtin_call(ast_node_t *node, context_t *context) {
    ast_call_node_t call_node = node->data->call_node;
    char *function_name = call_node.id_val;
    ast_node_t *arg1 = call_node.first_arg;

    if (strcmp(function_name, "readLine") == 0) {
        generate_readLine("GF@$builtin_return");
        printf("PUSHS GF@$builtin_return\n");
        return true;
    }
    else if (strcmp(function_name, "readInt") == 0) {
        generate_readInt("GF@$builtin_return", context);
        printf("PUSHS GF@$builtin_return\n");
        return true;
    }
    else if (strcmp(function_name, "readDouble") == 0) {
        generate_readDouble("GF@$builtin_return", context);
        printf("PUSHS GF@$builtin_return\n");
        return true;
    }
    else if (strcmp(function_name, "readBoolean") == 0) {
        generate_readBoolean("GF@$builtin_return");
        printf("PUSHS GF@$builtin_return\n");
        return true;
    }

    else if (strcmp(function_name, "print") == 0) {
        generate_print(arg1);
        printf("PUSHS nil@nil\n");
        return true;
    }
    else if (strcmp(function_name, "toStr") == 0) {
        generate_toStr("GF@$builtin_return", arg1);
        printf("PUSHS GF@$builtin_return\n");
        return true;
    }
    else if (strcmp(function_name, "toInt") == 0) {
        generate_toInt("GF@$builtin_return", arg1);
        printf("PUSHS GF@$builtin_return\n");
        return true;
    }
    else if (strcmp(function_name, "strLength") == 0) {
        generate_strLength("GF@$builtin_return", arg1);
        printf("PUSHS GF@$builtin_return\n");
        return true;
    }
    else if (strcmp(function_name, "chr") == 0) {
        generate_chr("GF@$builtin_return", arg1);
        printf("PUSHS GF@$builtin_return\n");
        return true;
    }
    
    else if (strcmp(function_name, "ord") == 0) {
        ast_node_t *arg2 = arg1->next;
        generate_ord("GF@$builtin_return", arg1, arg2);
        printf("PUSHS GF@$builtin_return\n");
        return true;
    }

    else if (strcmp(function_name, "subStr") == 0) {
        ast_node_t *arg2 = arg1->next;
        ast_node_t *arg3 = arg2->next;
        generate_subStr(arg1, arg2, arg3);
        return true;
    }
    return false;
}

void generate_IFJcode26(ast_node_t *root) {
    printf(".IFJcode26\n"); // Every code has to start with this notation
    printf("DEFVAR GF@$builtin_return\n");
    printf("CREATEFRAME\n");
    printf("CALL $%s\n", find_main_function(root)); // Make sure interpret starts processing code at main
    printf("EXIT int@0\n");
    generate_builtin_subStr();

    ast_node_t *node = root;
    context_t context = {0};
    context.root = root;
    while(node != NULL) {
        process_node(node, &context);
        node = node->next;
    }

    return;
}