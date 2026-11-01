/**
* @file expr_parser.h
* @internal Part of IFJ-project-2026-compiler, Team 274855
*
* @brief TODO
* @author Samuel Vít Lehotský (xlehots00)
* @date 18. 10. 2026
*/

#ifndef EXPR_PARSER_STACK_H
#define EXPR_PARSER_STACK_H

#include "ast.h"

typedef struct tstack_element {
    token_t token;
    TStack_Element *next;
} TStack_Element, *TStack_ElementPtr;

typedef struct tstack {
    TStack_Element *top;
} TStack;

void stack_init(TStack *stack);

void stack_push(TStack *stack, token_t *token);

void stack_push_after_top_terminal(TStack *stack, token_t *token);

void stack_pop(TStack *stack);

token_t top(TStack *stack);

token_t top_terminal(TStack *stack);

bool empty(TStack *stack);

void stack_dispose(TStack *stack);

#endif
