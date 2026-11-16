/**
* @file expr_parser.c
* @internal Part of IFJ-project-2026-compiler, Team 274855
*
* @brief TODO
* @author Samuel Vit Lehotsky (xlehots00)
* @date 3. 10. 2026
*/

#include "expr_parser.h"
#include "ast.h"
#include "scanner.h"
#include "error.h"

#define ELEMENT_ARRAY_SIZE 4

static const char precedence_table[PRECEDENCE_TABLE_SIZE][PRECEDENCE_TABLE_SIZE] = {
    /*         +-   * /  &&   ||   <><=>=   ==!=   (    )  -i!i   i    $   */
    /*  +-  */{'>', '<', '>', '>', '>',      '>', '<', '>', '<', '<', '>',},
    /*         +-   * /  &&   ||   <><=>=   ==!=   (    )  -i!i   i    $   */
    /* * /  */{'>', '>', '>', '>', '>',      '>', '<', '>', '<', '<', '>',},
    /*         +-   * /  &&   ||   <><=>=   ==!=   (    )  -i!i   i    $   */
    /*  &&  */{'<', '<', '>', '>', '<',      '<', '<', '>', '<', '<', '>',},
    /*         +-   * /  &&   ||   <><=>=   ==!=   (    )  -i!i   i    $   */
    /*  ||  */{'<', '<', '<', '>', '<',      '<', '<', '>', '<', '<', '>',},
    /*         +-   * /  &&   ||   <><=>=   ==!=   (    )  -i!i   i    $   */
    /*<><=>=*/{'<', '<', '>', '>', ' ',      '>', '<', '>', '<', '<', '>',},
    /*         +-   * /  &&   ||   <><=>=   ==!=   (    )  -i!i   i    $   */
    /* ==!= */{'<', '<', '>', '>', '<',      ' ', '<', '>', '<', '<', '>',},
    /*         +-   * /  &&   ||   <><=>=   ==!=   (    )  -i!i   i    $   */
    /*   (  */{'<', '<', '<', '<', '<',      '<', '<', '=', '<', '<', ' ',},
    /*         +-   * /  &&   ||   <><=>=   ==!=   (    )  -i!i   i    $   */
    /*   )  */{'>', '>', '>', '>', '>',      '>', ' ', '>', ' ', ' ', '>',},
    /*         +-   * /  &&   ||   <><=>=   ==!=   (    )  -i!i   i    $   */
    /* -i!i */{'>', '>', '>', '>', '>',      '>', '<', '>', '<', '<', '>',},
    /*         +-   * /  &&   ||   <><=>=   ==!=   (    )  -i!i   i    $   */
    /*   i  */{'>', '>', '>', '>', '>',      '>', ' ', '>', ' ', ' ', '>',},
    /*         +-   * /  &&   ||   <><=>=   ==!=   (    )  -i!i   i    $   */
    /*   $  */{'<', '<', '<', '<', '<',      '<', '<', ' ', '<', '<', ' ',},
};

precedence_table_index table_index(token_type type) {
    switch(type) {
        case T_PLUS:
        case T_MINUS:
            return P_ADD_SUB;
        
        case T_MULT:
        case T_DIV:
            return P_MUL_DIV;

        case T_AND:
            return P_AND;

        case T_OR:
            return P_OR;

        case T_LT:
        case T_LTE:
        case T_GT:
        case T_GTE:
            return P_LT_LTE_GT_GTE;

        case T_EQ:
        case T_NEQ:
            return P_EQ_NEQ;

        case T_L_ROUND_BRACKET:
            return P_L_ROUND_BRACKET;

        case T_R_ROUND_BRACKET:
            return P_R_ROUND_BRACKET;

        case EP_UNARY_NEG:
        case EP_UNARY_MINUS:
            return P_UNARY_NEG_MINUS;

        case T_IDENTIFIER:
        case T_INT_LITERAL:
        case T_DOUBLE_LITERAL:
        case T_BOOLEAN_LITERAL:
            return P_ID;

        default:
            return P_DOLLAR;
    }
}

bool is_operator(TStack_Element *array, int array_counter){
    return array[array_counter].token.type == T_PLUS   || array[array_counter].token.type == T_MINUS 
        || array[array_counter].token.type == T_MULT   || array[array_counter].token.type == T_DIV
        || array[array_counter].token.type == T_AND    || array[array_counter].token.type == T_OR
        || array[array_counter].token.type == T_LT     || array[array_counter].token.type == T_LTE
        || array[array_counter].token.type == T_GT     || array[array_counter].token.type == T_GTE
        || array[array_counter].token.type == T_EQ     || array[array_counter].token.type == T_NEQ;
}

//EXPRESSION PARSING

ast_node_t *parse_expression(token_t token)
{
    //Stack initialization 
    TStack stack;
    stack_init(&stack);

    //Adding $ character to sign the bottom of the stack
    token_t bottom = {0};
    bottom.type = EP_DOLLAR;
    stack_push(&stack, &bottom);

    TStack_Element array[ELEMENT_ARRAY_SIZE];
    int array_counter;
    
    token_t next_token = {0};
    next_token = token;

    token_type last_token = T_EOF;

    ast_node_t *tree = NULL;

    bool value = true;
    while (value) {

        if (next_token.type == T_MINUS || next_token.type == T_NEG) {
            bool valid = (last_token == T_IDENTIFIER || last_token == T_STRING_LITERAL || last_token == T_INT_LITERAL || last_token == T_DOUBLE_LITERAL ||
                          last_token == T_BOOLEAN_LITERAL || last_token == T_UNIT_LITERAL || last_token == T_R_ROUND_BRACKET);

            if (!valid) { next_token.type = (next_token.type == T_MINUS) ? EP_UNARY_MINUS : EP_UNARY_NEG; }
        }

        int row = table_index(top_terminal(&stack).type);
        int colm = table_index(next_token.type);

        switch (precedence_table[row][colm]) {
            case '<': {
                token_t token = {0};
                token.type = EP_LT_MARKER;
                stack_push_after_top_terminal(&stack, &token);
                stack_push(&stack, &next_token);
                last_token = next_token.type;
                next_token = get_next_token();
                break;
            }
            case '>':
                array_counter = 0;
                while(top(&stack).type != EP_LT_MARKER && array_counter < ELEMENT_ARRAY_SIZE) { // '<'
                    array[array_counter++] = *stack.top;
                    stack_pop(&stack);
                }
                stack_pop(&stack);
                grammar_check(&stack, array, array_counter);
                break;

            case '=': 
                stack_push(&stack, &next_token);
                last_token = next_token.type;
                next_token = get_next_token();
                break;

            default:
                if(top_terminal(&stack).type == EP_DOLLAR && (next_token.type == T_EOF || next_token.type == T_LF || next_token.type == T_SEMICOLON ||
                                                              next_token.type == T_KEY_THEN || next_token.type ==  T_KEY_DO)) {
                    if (next_token.type == T_KEY_THEN || next_token.type ==  T_KEY_DO) {
                        return_buffer_token(next_token);
                    }
                    tree = top(&stack).value.ast_node_ptr;
                    value = false;
                    break;
                }
                else {
                    error_exit(SYNT_ERR, next_token.line_num, "Invalid token or operator sequence in expression."); //TODO ERROR
                }
        } 
    } 

    stack_dispose(&stack);
    return tree;
}

void grammar_check(TStack *stack, TStack_Element *array, int array_counter) {
    token_t new_token = {0};
    new_token.type = EP_EXPR;
    new_token.line_num = array[0].token.line_num;
    switch(array_counter) {
        case 1:
            if (array[0].token.type == T_IDENTIFIER) {
                new_token.value.ast_node_ptr = expr_insert_id_leaf(array[0].token.value.string_value, array[0].token.line_num);
                stack_push(stack, &new_token);
            }
            else if (array[0].token.type == T_INT_LITERAL || array[0].token.type == T_DOUBLE_LITERAL || array[0].token.type == T_BOOLEAN_LITERAL) {
                new_token.value.ast_node_ptr = expr_insert_lit_leaf(array[0].token.value, array[0].token.line_num, array[0].token.type);
                stack_push(stack, &new_token);
            }
            else { error_exit (SYNT_ERR, array[0].token.line_num, "Invalid operand: expected identifier or literal."); } //TODO ERROR
            break;

        case 2:
            if (array[0].token.type == T_R_ROUND_BRACKET && array[1].token.type == T_L_ROUND_BRACKET) {
                new_token.value.ast_node_ptr = expr_insert_lit_leaf(array[0].token.value, array[0].token.line_num, T_UNIT_LITERAL);
                stack_push(stack, &new_token);
            }
            else if (array[0].token.type == EP_EXPR && (array[1].token.type == EP_UNARY_MINUS || array[1].token.type == EP_UNARY_NEG)) {
                new_token.value.ast_node_ptr = expr_insert_op(array[1].token.type, array[1].token.line_num, NULL, array[0].token.value.ast_node_ptr);
                stack_push(stack, &new_token);
            }
            else { error_exit (SYNT_ERR, array[0].token.line_num, "Invalid unary expression or mismatched parentheses."); } //TODO ERROR
            break;

        case 3:
            if (array[0].token.type == EP_EXPR && (is_operator(array, 1)) && array[2].token.type == EP_EXPR) {
                new_token.value.ast_node_ptr = expr_insert_op(array[1].token.type, array[1].token.line_num, array[2].token.value.ast_node_ptr, array[0].token.value.ast_node_ptr);
                stack_push(stack, &new_token);
            }
            else if (array[0].token.type == T_R_ROUND_BRACKET && array[1].token.type == EP_EXPR && array[2].token.type == T_L_ROUND_BRACKET) {
                new_token.value.ast_node_ptr = array[1].token.value.ast_node_ptr;
                stack_push(stack, &new_token);
            }
            else { error_exit (SYNT_ERR, array[0].token.line_num, "Invalid binary expression or mismatched parentheses."); } //TODO ERROR
            break;

        default:
            error_exit(SYNT_ERR, array[0].token.line_num, "Invalid expression reduction rule."); //TODO ERROR
            break;
    }
}
