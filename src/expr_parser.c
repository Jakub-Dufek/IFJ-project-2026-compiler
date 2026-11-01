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
    /*        +-    /*   (    )    i    $   */
    /*  +- */{'>', '<', '<', '>', '<', '>'},
    /*  /* */{'>', '>', '<', '>', '<', '>'},
    /*  (  */{'>', '>', '<', '=', '<', '>'},
    /*  )  */{'>', '>', ' ', '>', '<', '>'},
    /*  i  */{'>', '>', ' ', '>', ' ', '>'},
    /*  $  */{'<', '<', '<', ' ', '<', ' '}
};

precedence_table_index table_index(token_type type) {
    switch(type) {
        case T_PLUS:
        case T_MINUS:
            return P_ADD_SUB;
        
        case T_MULT:
        case T_DIV:
            return P_MUL_DIV;
        
        case T_L_ROUND_BRACKET:
            return P_L_ROUND_BRACKET;

        case T_R_ROUND_BRACKET:
            return P_R_ROUND_BRACKET;

        case T_IDENTIFIER:
            return P_ID;

        default:
            return P_DOLLAR;
    }
}

static inline bool is_operator(TStack_Element *array, int array_counter){
    return array[array_counter].token.type == T_PLUS   || array[array_counter].token.type == T_MINUS 
        || array[array_counter].token.type == T_MULT   || array[array_counter].token.type == T_DIV
        || array[array_counter].token.type == T_GT     || array[array_counter].token.type == T_GTE
        || array[array_counter].token.type == T_LT     || array[array_counter].token.type == T_LTE
        || array[array_counter].token.type == T_EQ     || array[array_counter].token.type == T_NEQ
        || array[array_counter].token.type == T_ASSIGN;
}

//EXPRESSION PARSING

ast_node_t *parse_expression(token_t token)
{
    //Stack initialization 
    TStack stack;
    stack_init(&stack);

    //Adding $ character to sign the bottom of the stack
    token_t bottom;
    bottom.type = EP_DOLLAR;
    stack_push(&stack, &bottom);

    TStack_Element array[ELEMENT_ARRAY_SIZE];
    int array_counter;
    
    token_t next_token = token;

    while (true) {
        int row = table_index(top_terminal(&stack).type);
        int colm = table_index(next_token.type);

        switch (precedence_table[row][colm]) {
            case '<':
                token_t token;
                token.type = EP_LT_MARKER;
                stack_push_after_top_terminal(&stack, &token);
                stack_push(&stack, &next_token);
                next_token = get_next_token();
                break;

            case '>':
                array_counter = 0;
                while(top(&stack).type != EP_LT_MARKER) { // '<'
                    array[array_counter++] = *stack.top;
                    stack_pop(&stack);
                }
                stack_pop(&stack);
                grammar_check(&stack, array, array_counter);
                break;

            case '=': 
                stack_push(&stack, &next_token);
                next_token = get_next_token();
                break;

            default:
                if(top_terminal(&stack).type == EP_DOLLAR && next_token.type == T_EOF) { //TODO next_token
                    return(top(&stack).value.ast_node_ptr);
                }
                else {
                    error_exit(SYNT_ERR, next_token.line_num, ""); //TODO ERROR
                }
        } 
    } 

    stack_dispose(&stack);
}

void grammar_check(TStack *stack, TStack_Element *array, int array_counter) {
    token_t new_token;
    new_token.type = EP_EXPR;
    switch(array_counter) {
        case 1:
            if (array[0].token.type == T_IDENTIFIER) {
                new_token.value.ast_node_ptr = expr_insert_id_leaf(array[0].token.value.string_value, array[0].token.line_num);
                stack_push(stack, &new_token);
            }
            else if (array[0].token.type == T_INT_LITERAL || array[0].token.type == T_DOUBLE_LITERAL) {
                new_token.value.ast_node_ptr = expr_insert_lit_leaf(array[0].token.value, array[0].token.line_num, array[0].token.type);
                stack_push(stack, &new_token);
            }
            else { error_exit (SYNT_ERR, new_token.line_num, ""); } //TODO ERROR
            break;

        case 2:
            if (array[0].token.type == T_R_ROUND_BRACKET && array[1].token.type == T_L_ROUND_BRACKET) {
                new_token.value.ast_node_ptr = expr_insert_lit_leaf(array[0].token.value, array[0].token.line_num, T_UNIT_LITERAL);
                stack_push(stack, &new_token);
            }
            else { error_exit (SYNT_ERR, new_token.line_num, ""); } //TODO ERROR
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
            else { error_exit (SYNT_ERR, new_token.line_num, ""); } //TODO ERROR
            break;

        default:
            error_exit(SYNT_ERR, new_token.line_num, ""); //TODO ERROR
            break;
    }
}
