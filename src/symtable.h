/**
 * @file symtable.h
 * @internal Part of IFJ-project-2026-compiler, Team 274855
 *
 * @brief Symbol table structure and operations
 * @author Ondřej Kycl (xkyclon00) 
 * @date 04. 10. 2026
 * 
 * @details This file contains declarations and descriptions of structures 
 * and functions used in symtable.c for managing variables, values, and functions.
 */

#ifndef SYMTABLE_H
#define SYMTABLE_H

#include <stdbool.h>
#include <stdlib.h>
#include <string.h> 

#include "structures.h"
#include "memory.h"
#include "../src/error.h"



// SYMTABLE STRUCTURES & ENUMS


/**
 * @brief More convenient rename for symbol table data types.
 * 
 * @details Description of data types with enum from "structures.h":
 * 0-6 - (T_UNDEFINED = NO_VALUE = 0, T_KEY_INT = 1, T_KEY_DOUBLE = 2,
 * T_KEY_STRING = 3, T_KEY_BOOLEAN = 4, T_KEY_UNIT = 5, T_NO_TYPE = 6).
 * token_type is renamed to data_type to match situation better.
 */
typedef token_type data_type;

/**
 * @brief Description of an ID type of a symbol with enum.
 */
typedef enum {
    ID_VAL,
    ID_VAR,
    ID_FUNC,
} id_type;



// LINKED LIST STRUCTURES (Function Parameters)


/**
 * @brief Element of linked list - data_type.
 */
typedef struct param_type {
    data_type type;
    struct param_type *next_ptr;
} param_type_t;

/**
 * @brief Linked list consisting of function parameter's data types.
 */
typedef struct param_type_list {
    param_type_t *first;
    param_type_t *last;
} param_type_list_t;



// SYMBOL INFORMATION STRUCTURES


/**
 * @brief Information about a function.
 */
typedef struct {
    unsigned param_num;
    param_type_list_t *param_types;
    bool is_defined;
    bool is_main;
} func_info_t;

/**
 * @brief symbol_data_t is a structure for storage of data related to a symbol.
 */
typedef struct {
    data_type data_type; 
    id_type id_type;
    func_info_t func_info;
} symbol_data_t;

/**
 * @brief A node of a binary tree.
 */
typedef struct symtable_bvs_node_t {
    char *key;                               // identifier value used for binary search
    symbol_data_t symbol_data;
    struct symtable_bvs_node_t *l_ptr;
    struct symtable_bvs_node_t *r_ptr;
    int weight;
    unsigned height;
} symtable_bvs_node_t;   



// STACK STRUCTURES


/**
 * @brief A symtable instance on the stack.
 */
typedef struct symtable_instance_t {
    symtable_bvs_node_t *symtable;
    struct symtable_instance_t *next;
} symtable_instance_t;

/**
 * @brief Stack of symtables.
 */
typedef struct {
    symtable_instance_t *top;
} symtable_stack_t;


// ============================================================================
// SYMTABLE FUNCTIONS
// ============================================================================

/**
 * @brief Initialization of the symbol table.
 * 
 * @return symtable_bvs_node_t* Pointer to the initialized root.
 */
symtable_bvs_node_t *symtable_initialize();

/**
 * @brief symtable_insert helper function - returns greater number of the two.
 */
int bvs_max(int n1, int n2);

/**
 * @brief symtable_insert helper function - returns height from the current pointer.
 */
int bvs_height(symtable_bvs_node_t *ptr);

/**
 * @brief symtable_insert helper function - performs AVL tree rotation to the left.
 * 
 * @details Uses bvs_max and bvs_height.
 */
symtable_bvs_node_t *symtable_left_rotate(symtable_bvs_node_t *y);

/**
 * @brief symtable_insert helper function - performs AVL tree rotation to the right.
 * 
 * @details Uses bvs_max and bvs_height.
 */
symtable_bvs_node_t *symtable_right_rotate(symtable_bvs_node_t *y);

/**
 * @brief Returns the position of the found symbol node.
 * 
 * @details Returns the position of the found symbol node, or NULL if not found.
 * Implementation is recursive.
 * 
 * @param rootPtr Root of the binary tree to search in.
 * @param k Key (identifier) to search for.
 * @return symtable_bvs_node_t* Pointer to the found node or NULL.
 */
symtable_bvs_node_t *symtable_find_symbol(symtable_bvs_node_t *rootPtr, char *k);

/**
 * @brief symtable_insert helper function - allocates a new node.
 * 
 * @details Allocates a new node, allocates key string and inserts received 
 * symbol information into the node.
 */
symtable_bvs_node_t *symtable_create_node(char *key, symbol_data_t symb);

/**
 * @brief Inserts new node as a leaf node of a binary tree.
 * 
 * @details Performs height balancing through rotations if necessary, 
 * uses error() if insertion is not possible. Implementation is recursive.
 * 
 * @param rootPtr Current root of the tree/subtree.
 * @param key Key (identifier) of the new symbol.
 * @param symb Symbol data to insert.
 * @return symtable_bvs_node_t* Pointer to the new root of the balanced subtree.
 */
symtable_bvs_node_t *symtable_insert(symtable_bvs_node_t *rootPtr, char *key, symbol_data_t symb);

/**
 * @brief Frees the symbol table and all its nodes.
 * 
 * @param root_ptr Root of the binary tree to destroy.
 */
void symtable_destructor(symtable_bvs_node_t *root_ptr);


// ============================================================================
// STACK FUNCTIONS
// ============================================================================

/**
 * @brief Initialization of the symbol table stack.
 */
void symtable_stack_initialize(symtable_stack_t *stack);

/**
 * @brief Puts new symbol table instance on top of the stack.
 */
void symtable_stack_push(symtable_stack_t *stack);

/**
 * @brief Deletes the symbol table instance from top of the stack.
 */
void symtable_stack_pop(symtable_stack_t *stack);

/**
 * @brief Function searches for a symbol in all instances on the stack.
 * 
 * @details Traverses the stack from top to bottom and returns the location 
 * of the first matching symbol found.
 * 
 * @param stack Pointer to the stack to search in.
 * @param key Key (identifier) to search for.
 * @return symtable_bvs_node_t* Pointer to the found node or NULL.
 */
symtable_bvs_node_t *symtable_stack_search(symtable_stack_t *stack, char *key);


// ============================================================================
// LINKED LIST FUNCTIONS (Parameter Handling)
// ============================================================================

/**
 * @brief Initialization of the parameter list.
 */
param_type_list_t *param_type_list_initialize(void);

/**
 * @brief Inserts and allocates new parameter's data type as the last element.
 */
void param_type_list_insert(param_type_list_t *l, data_type type);

/**
 * @brief Frees entire parameter's types list.
 * 
 * @note Uncomment and implement in symtable.c when necessary.
 */
// void param_list_destroy(param_type_list_t *l);

#endif