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
            break;
        }
        break;
    
    case EXPR_IDENTIFIER:
        type = node->data->id_leaf_node.symbol_data.data_type;
        break;

    case EXPR_INT2FLOAT:
        type = T_KEY_DOUBLE;
        break;
    
    case EXPR_OPERATION:
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

    default:
        break;
    }

    return type;
}

void generate_operation(ast_node_t *node) {
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

    return;
}

void generate_identifier(ast_node_t *node) {
    // #TODO implementace

    return;
}

void generate_literal(ast_node_t *node, char *print_before) {
    if (print_before != NULL) {
        printf("%s", print_before);
    }

    switch (node->data->lit_leaf_node.lit_type)
    {
    case T_STRING_LITERAL:
        printf("string@%s\n", node->data->lit_leaf_node.lit_value.string_value);
        break;
    
    case T_INT_LITERAL:
        printf("int@%i\n", node->data->lit_leaf_node.lit_value.int_value);
        break;

    case T_DOUBLE_LITERAL:
        printf("float@%a\n", node->data->lit_leaf_node.lit_value.double_value);
        break;

    case T_UNIT_LITERAL:
        printf("nil@nil");
        break;

    default:
        // #TODO error, nerozpoznaný typ literálu
        break;
    }

    return;
}

void process_node(ast_node_t *node, char *message_before) {
    switch (node->stmt_type)
    {
    case AST_BLOCK:
        generate_block(node);
        break;

    case AST_DECLARATION:
        generate_declaration(node);
        break;

    case AST_DEFINITION:
       generate_function(node);
        break;

    case EXPR_OPERATION:
        generate_operation(node);
        break;

    case EXPR_IDENTIFIER:
        generate_identifier(node);
        break;

    case EXPR_LITERAL:
        generate_literal(node, message_before);
        break;

    case EXPR_INT2FLOAT:
        /* code */
        break;
    
    default:
        // #TODO error, nerozpoznaný typ uzlu
        break;
    }

    return;
}

void generate_IFJcode26(ast_node_t *root) {
    printf(".IFJcode26\n"); //Every code has to start with this notation

    ast_node_t *node = root;
    while(node != NULL) {
        process_node(node, NULL);
        node = node->next;
    }

    return;
}