/**
 * @file expr_parser_stack.c
 * @internal Part of IFJ-project-2026-compiler, Team 274855
 *
 * @brief Fuctions for expression parser stack
 * @author Samuel Vit Lehotsky (xlehots00)
 * @date 18. 10. 2026
 */

#include "expr_parser_stack.h"
#include "ast.h"
#include "error.h"
#include "expr_parser.h"
#include "scanner.h"

void stack_init(TStack *stack) { stack->top = NULL; }

void stack_push(TStack *stack, token_t *token)
{
    TStack_Element *new_element = (TStack_ElementPtr)malloc(sizeof(TStack_Element));
    if(new_element == NULL) {
        error_exit(INTERNAL_ERR, token->line_num, "Internal memmory allocation error.");
    }
    new_element->token = *token;
    new_element->next = stack->top;
    stack->top = new_element;
}

void stack_push_after_top_terminal(TStack *stack, token_t *token)
{
    TStack_ElementPtr temp_element = stack->top;
    TStack_ElementPtr prev_element = temp_element;
    while(temp_element != NULL) {
        if(temp_element->token.type != EP_EXPR && temp_element->token.type != EP_LT_MARKER) {
            TStack_Element *new_element = (TStack_ElementPtr)malloc(sizeof(TStack_Element));
            if(new_element == NULL) {
                error_exit(INTERNAL_ERR, token->line_num, "Internal memmory allocation error.");
            }
            if(temp_element == stack->top) {
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

void stack_pop(TStack *stack)
{
    TStack_Element *temp_element;
    if(!empty(stack)) {
        temp_element = stack->top;
        stack->top = stack->top->next;
        free(temp_element);
    }
}

token_t top(TStack *stack)
{
    if(empty(stack)) {
        error_exit(INTERNAL_ERR, 0, "Internal expression parser stack error.");
    }
    return (stack->top->token);
}

token_t top_terminal(TStack *stack)
{
    if(empty(stack)) {
        error_exit(INTERNAL_ERR, 0, "Internal expression parser stack error.");
    }
    TStack_ElementPtr temp_element = stack->top;
    while(temp_element != NULL) {
        if(temp_element->token.type != EP_EXPR && temp_element->token.type != EP_LT_MARKER) {
            return temp_element->token;
        }
        temp_element = temp_element->next;
    }
    error_exit(INTERNAL_ERR, 0, "No terminal found on expression parser stack.");
    token_t token = {0};
    return token;
}

bool empty(TStack *stack) { return (stack->top == NULL); }

void stack_dispose(TStack *stack)
{
    while(!empty(stack)) {
        TStack_Element *temp_element = stack->top;
        stack->top = temp_element->next;
        free(temp_element);
    }
}