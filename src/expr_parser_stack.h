/**
* @file expr_parser_stack.h
* @internal Part of IFJ-project-2026-compiler, Team 274855
*
* @brief Header file for expression parser stack
* @author Samuel Vít Lehotský (xlehots00)
* @date 18. 10. 2026
*/

#ifndef EXPR_PARSER_STACK_H
#define EXPR_PARSER_STACK_H

#include "ast.h"

typedef struct tstack_element {
    token_t token;
    struct tstack_element *next;
} TStack_Element, *TStack_ElementPtr;

typedef struct tstack {
    TStack_Element *top;
} TStack;


/**
 * @brief Initializes the stack
 */
void stack_init(TStack *stack);


/**
 * @brief Insertes an element onto the top of the stack
 *
 * @param token Pointer to the token to be inserted
 */
void stack_push(TStack *stack, token_t *token);


/**
 * @brief Inserts an element right after the top terminal on the stack
 *
 * @param token Pointer to the token to be inserted
 */
void stack_push_after_top_terminal(TStack *stack, token_t *token);


/**
 * @brief Removes the element from the top of the stack
 */
void stack_pop(TStack *stack);


/**
 * @brief Returns the element on the top of the stack
 *
 * @return Token on the top of the stack
 */
token_t top(TStack *stack);


/**
 * @brief Return the highest terminal on the stack
 *
 * @return Token corresponding to the highest terminal on the stack
 */
token_t top_terminal(TStack *stack);


/**
 * @brief Checks if the stack is empty
 * 
 * @return true if the stack is empty
 */
bool empty(TStack *stack);


/**
 * @brief Disposes of the stack and frees allocated memory
 */
void stack_dispose(TStack *stack);

#endif
