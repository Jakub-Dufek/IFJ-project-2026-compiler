/**
* @file expr_parser.c
* @internal Part of IFJ-project-2026-compiler, Team 274855
*
* @brief TODO
* @author Samuel Vit Lehotsky (xlehots00)
* @date 18. 10. 2026
*/

#include "expr_parser.h"
#include "ast.h"
#include "scanner.h"
#include "expr_parser_stack.h"

void stack_init(TStack *stack) {
    stack->top = NULL;
}

void stack_push(TStack *stack, token_t *token) {
    TStack_Element *new_element = (TStack_ElementPtr)malloc(sizeof(TStack_Element));
    if (new_element == NULL) { error_code(INTERNAL_ERR); }
    new_element->token = *token;
    new_element->next = stack->top;
    stack->top = new_element;
}

void stack_push_after_top_terminal(TStack *stack, token_t *token) { //TODO
    TStack_ElementPtr temp_element = stack->top;
    TStack_ElementPtr prev_element = temp_element;
    while (temp_element->token.type != NULL) {
        if (temp_element->token.type != EP_EXPR) {
            TStack_Element *new_element = (TStack_ElementPtr)malloc(sizeof(TStack_Element));
            if (new_element == NULL) { error_code(INTERNAL_ERR); }
            if (temp_element == stack->top) {
                new_element->next = stack->top;
                stack->top = new_element;
            }
            else {
                prev_element->next = new_element;
                new_element->next = temp_element;
            }
            new_element->token = *token;
            break;
        }
        prev_element = temp_element;
        temp_element = temp_element->next;
    }
}

void stack_pop(TStack *stack) {
    TStack_Element *temp_element;
    if (!empty(stack)) {
        temp_element = stack->top;
        stack->top = stack->top->next;
        free(temp_element);
    }
}

token_t top(TStack *stack) {
    if (empty(stack)) { error_code(INTERNAL_ERR); } //?
    return (stack->top->token);
}

token_t top_terminal(TStack *stack) {
    if (empty(stack)) { error_code(INTERNAL_ERR); } //?
    TStack_ElementPtr temp_element = stack->top;
    while (temp_element->token.type != EP_EXPR) {
        if (temp_element->token.value.ast_node_ptr == NULL) {
            return temp_element->token;
        }
        temp_element = temp_element->next;
    }
    return temp_element->token;
}

bool empty(TStack *stack) { return (stack->top == NULL); }

void stack_dispose(TStack *stack) {
    while (!empty(stack)) {
        TStack_Element *temp_element = stack->top;
        stack->top = temp_element->next;
        free(temp_element);
    }
}