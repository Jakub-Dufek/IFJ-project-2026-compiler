#include <stdio.h>
#include <string.h>
#include "ast.h"
#include "structures.h"

// -----------------------------------------------------------------------------
// HELPER: TOKEN TO STRING CONVERTER
// -----------------------------------------------------------------------------

/**
 * @brief Converts token_type enum to its string representation
 */
static const char *token_type_to_string(token_type type) {
    switch (type) {
        case T_UNDEFINED: return "T_UNDEFINED";
        case T_KEY_INT: return "T_KEY_INT";
        case T_KEY_DOUBLE: return "T_KEY_DOUBLE";
        case T_KEY_STRING: return "T_KEY_STRING";
        case T_KEY_BOOLEAN: return "T_KEY_BOOLEAN";
        case T_KEY_UNIT: return "T_KEY_UNIT";
        case T_NO_TYPE: return "T_NO_TYPE";
        
        case T_KEY_DEF: return "T_KEY_DEF";
        case T_KEY_DO: return "T_KEY_DO";
        case T_KEY_ELSE: return "T_KEY_ELSE";
        case T_KEY_IF: return "T_KEY_IF";
        case T_KEY_IMPORT: return "T_KEY_IMPORT";
        case T_KEY_RETURN: return "T_KEY_RETURN";
        case T_KEY_THEN: return "T_KEY_THEN";
        case T_KEY_VAL: return "T_KEY_VAL";
        case T_KEY_VAR: return "T_KEY_VAR";
        case T_KEY_WHILE: return "T_KEY_WHILE";
        
        case T_PLUS: return "T_PLUS";
        case T_MINUS: return "T_MINUS";
        case T_MULT: return "T_MULT";
        case T_DIV: return "T_DIV";
        case T_GT: return "T_GT";
        case T_GTE: return "T_GTE";
        case T_LT: return "T_LT";
        case T_LTE: return "T_LTE";
        case T_EQ: return "T_EQ";
        case T_NEQ: return "T_NEQ";
        case T_ASSIGN: return "T_ASSIGN";
        
        case T_IDENTIFIER: return "T_IDENTIFIER";
        case T_STRING_LITERAL: return "T_STRING_LITERAL";
        case T_INT_LITERAL: return "T_INT_LITERAL";
        case T_DOUBLE_LITERAL: return "T_DOUBLE_LITERAL";
        case T_UNIT_LITERAL: return "T_UNIT_LITERAL";
        
        // Add remaining as needed, these are the most common for AST
        default: return "UNKNOWN_TOKEN_TYPE";
    }
}


// -----------------------------------------------------------------------------
// AST DUMPER FUNCTIONS
// -----------------------------------------------------------------------------

/**
 * @brief Helper function for visual indentation based on tree depth
 */
static void print_indent(int level) {
    for (int i = 0; i < level; i++) {
        printf("  | ");
    }
}

/**
 * @brief Main recursive function for printing the AST structure
 */
void ast_print(ast_node_t *node, int level) {
    if (node == NULL) {
        return;
    }

    print_indent(level);
    
    switch (node->stmt_type) {
        case AST_BLOCK:
            printf("[AST_BLOCK] (line: %u)\n", node->line_num);
            // Print the first statement of the block
            ast_print(node->data->block_node.first, level + 1);
            break;

        case AST_DECLARATION:
            printf("[AST_DECLARATION] id: '%s', mutable: %s, type: %s (line: %u)\n", 
                   node->data->declaration_node.id_val,
                   node->data->declaration_node.is_mutable ? "true" : "false",
                   token_type_to_string(node->data->declaration_node.data_type),
                   node->line_num);
            // Print the right-side expression of the declaration
            ast_print(node->data->declaration_node.expression, level + 1);
            break;

        case AST_DEFINITION:
            printf("[AST_DEFINITION] func: '%s', return_type: %s, main: %s (line: %u)\n",
                   node->data->definition_node.id_val,
                   token_type_to_string(node->data->definition_node.data_type),
                   node->data->definition_node.is_main ? "true" : "false",
                   node->line_num);
            
            // Print parameters if they exist
            if (node->data->definition_node.first_param != NULL) {
                print_indent(level + 1);
                printf("--- Parameters ---\n");
                ast_print(node->data->definition_node.first_param, level + 2);
            }
            
            // Print function body
            print_indent(level + 1);
            printf("--- Body ---\n");
            ast_print(node->data->definition_node.body, level + 2);
            break;

        case AST_PARAM:
            printf("[AST_PARAM] id: '%s', type: %s\n", 
                   node->data->param_node.id_val, 
                   token_type_to_string(node->data->param_node.data_type));
            break;

        case EXPR_OPERATION:
            printf("[EXPR_OPERATION] op_type: %s (line: %u)\n", 
                   token_type_to_string(node->data->op_node.op_type), 
                   node->line_num);
            ast_print(node->data->op_node.left, level + 1);
            ast_print(node->data->op_node.right, level + 1);
            break;

        case EXPR_IDENTIFIER:
            printf("[EXPR_IDENTIFIER] id: '%s' (line: %u)\n", 
                   node->data->id_leaf_node.id_val, node->line_num);
            break;

        case EXPR_LITERAL:
            printf("[EXPR_LITERAL] type: %s (line: %u)\n", 
                   token_type_to_string(node->data->lit_leaf_node.lit_type), 
                   node->line_num);
            break;

        case EXPR_INT2FLOAT:
            printf("[EXPR_INT2FLOAT] Implicit conversion (line: %u)\n", node->line_num);
            ast_print(node->data->imp_conversion.operand, level + 1);
            break;

        default:
            printf("[UNKNOWN_NODE]\n");
            break;
    }

    // Recursive call for the next statement in the same block or next parameter
    if (node->next != NULL) {
        ast_print(node->next, level);
    }
}