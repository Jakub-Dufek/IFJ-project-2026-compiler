/**
 * @file IFJcode_generator.c
 * @internal Part of IFJ-project-2026-compiler
 *
 * @brief FSM for code generator
 * @author Šimon Greger (xgreges00)
 * @date 25.10.2026
 *
 * @details This file contains finite state machine representing the functions of a IFJcode26 generator
 */

#include "IFJcode_generator.h"
#include "ast.h"
#include <stdio.h>


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
    if (!flag) {putchar('\n');}
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

        token_type type_left = find_operand_type(node->data->op_node.left);
        token_type type_right = find_operand_type(node->data->op_node.right);
        if (type_left == T_KEY_INT && type_right == T_KEY_INT) {
            type = T_KEY_INT;
        }
        else if (type_left == T_KEY_DOUBLE || type_right == T_KEY_DOUBLE) {
            type = T_KEY_DOUBLE;
        }
        else {
            // #TODO pro stringy, kdyby bylo potřeba
        }
        break;
    }
    default:
        break;
    }

    return type;
}


void generate_block(ast_node_t *node) {
    ast_node_t *block_node = node->data->block_node.first;
    while (block_node != NULL) {
        process_node(block_node, NULL);
        block_node = block_node->next;
    }
}

void generate_declaration(ast_node_t *node, char *frame) {
    printf("DEFVAR %s@%s\n", frame, node->data->declaration_node.id_val);
    process_node(node->data->declaration_node.expr_or_call, "PUSHS ");
    printf("POPS %s@%s\n", frame, node->data->declaration_node.id_val);
}

void generate_assignment(ast_node_t *node) {
    process_node(node->data->assign_node.expr_or_call, "PUSHS ");
    printf("POPS LF@%s\n", node->data->assign_node.id_val);
}

void generate_function(ast_node_t *node) {
    ast_definition_node_t def_node = node->data->definition_node;

    printf("LABEL $%s\n", def_node.id_val);
    printf("PUSHFRAME\n");

    generate_block(def_node.body);

    printf("POPFRAME\n");
    printf("RETURN\n");
}

void generate_call(ast_node_t *node) {
    ast_call_node_t call_node = node->data->call_node;
    int arg_count = 0;

    for (ast_node_t *arg = call_node.first_arg; arg != NULL; arg = arg->next) {
        process_node(arg, "PUSHS ");
        arg_count++;
    }

    printf("CREATEFRAME\n");
    
    for (int counter = 0; counter < arg_count; counter++) {
        printf("DEFVAR TF@arg%i\n", counter);
        printf("POPS TF@arg%i\n", counter);
    }

    printf("CALL $%s\n", call_node.id_val);
}

void generate_operation(ast_node_t *node) {
    // Special handling for unary minus, because all other operations are binary
    if (node->data->op_node.op_type == EP_UNARY_MINUS) {
        // Push a zero on the stack
        if (find_operand_type(node) == T_KEY_INT) {
            printf("PUSHS int@0\n");
        }
        else {
            printf("PUSHS float@0x0p+0\n");
        }

        process_node(node->data->op_node.right, "PUSHS ");
        printf("SUBS\n");
        return;
    } 
    else if (node->data->op_node.op_type == EP_UNARY_NEG) {
        process_node(node->data->op_node.right, "PUSHS ");
        printf("NOTS\n");
        return;
    }

    process_node(node->data->op_node.left, "PUSHS ");
    process_node(node->data->op_node.right, "PUSHS ");

    //Print the type of the operation
    switch (node->data->op_node.op_type)
    {
    case T_PLUS:
        printf("ADDS\n");
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

void generate_identifier(ast_node_t *node, char *print_before) {
    if (print_before != NULL) {
        printf("%s", print_before);
    }

    printf("LF@%s\n", node->data->id_leaf_node.id_val);
    //TODO... dokončit funkci
}

void generate_literal(ast_node_t *node, char *print_before) {
    if (print_before != NULL) {
        printf("%s", print_before);
    }

    switch (node->data->lit_leaf_node.lit_type)
    {
    case T_STRING_LITERAL:
        print_string_literal(node->data->lit_leaf_node.lit_value.string_value, false);
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

    default:
        // #TODO error, nerozpoznaný typ literálu
        break;
    }
}

void process_node(ast_node_t *node, char *message_before) {
    switch (node->stmt_type)
    {
    case AST_BLOCK:
        generate_block(node);
        break;

    case AST_DECLARATION:
        generate_declaration(node, "LF");   // TODO mozna neni vzdy LF
        break;

    case AST_DEFINITION:
       generate_function(node);
        break;

    case EXPR_OPERATION:
        generate_operation(node);
        break;

    case EXPR_IDENTIFIER:
        generate_identifier(node, message_before);
        break;

    case EXPR_LITERAL:
        generate_literal(node, message_before);
        break;

    case EXPR_INT2FLOAT:
        process_node(node->data->imp_conversion.operand, "PUSHS ");
        printf("INT2FLOATS\n");
        break;

    case AST_ASSIGN:
        generate_assignment(node);
        break;

    case AST_CALL:
        generate_call(node);
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

                case T_UNIT_LITERAL:
                    printf("WRITE string@\\032()\\032\n");  // Mezery navíc kolem závorek?
                    break;

                default:
                    //TODO ERROR
                    break;
            }
            break;

        case EXPR_IDENTIFIER:
            break;

        default:
            //TODO ERRO
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

void generate_IFJcode26(ast_node_t *root) {
    printf(".IFJcode26\n"); // Every code has to start with this notation
    printf("CREATEFRAME\n");
    printf("CALL $%s\n", find_main_function(root)); // Make sure interpret starts processing code at main
    printf("EXIT int@0\n");

    ast_node_t *node = root;
    while(node != NULL) {
        process_node(node, NULL);
        node = node->next;
    }

    return;
}