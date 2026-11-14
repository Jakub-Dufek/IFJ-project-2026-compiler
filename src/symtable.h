/**
* @file symtable.h
* @internal Part of IFJ-project-2026-compiler
*
* @brief symbol table structure
* @author Ondřej Kycl (xkyclon00) 
* @date 04.10.2026
* 
* @details This file contains declarations and descriptions of functions used in symtable.c
*/
#ifndef SYMTABLE_H
#define SYMTABLE_H

//libraries:
#include "structures.h"
#include "memory.h"
#include "error.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h> 

//definitions:


//SYMTABLE STRUCTURES

/* 
 description of data types with enum from "structures.h":
 0-6 - (T_UNDEFINED = NO_VALUE = 0, T_KEY_INT = 1, T_KEY_DOUBLE = 2,
 T_KEY_STRING = 3, T_KEY_BOOLEAN = 4, T_KEY_UNIT = 5, T_NO_TYPE = 6), 
 token_type is renamed to data_type to match situation better 
*/
typedef token_type data_type; // More convenient rename for symbol table

/* 
 id_type description of an id type of a symbol with enum
*/
typedef enum {
    ID_VAL,
    ID_VAR,
    ID_FUNC,
} id_type;


//LINKED LIST STRUCTURES

/*
 element of linked list - data_type
*/
typedef struct param_type {
    data_type type;
    struct param_type *next_ptr;
} param_type_t;

/*
 linked list constisting of function parameter's data types
*/
typedef struct param_type_list {
    param_type_t *first;
    param_type_t *last;
} param_type_list_t;


//SYMBOL INFORMATION STRUCTURES

/*
 information about a function
*/
typedef struct {
    unsigned param_num;
    param_type_list_t *param_types;
    bool is_defined;
    bool is_main;
} func_info_t;
/* 
 symbol_data_t is a structure for storage of data related to a symbol
*/
typedef struct {
    data_type data_type; 
    id_type id_type;

    func_info_t func_info;
} symbol_data_t;

/* 
 A node of a binary tree
*/
typedef struct symtable_bvs_node_t {
    char *key; // identifier value used for binary search
    symbol_data_t symbol_data;
    struct symtable_bvs_node_t *l_ptr;
    struct symtable_bvs_node_t *r_ptr;
    int weight;
    unsigned height;
} symtable_bvs_node_t;   


//STACK STRUCTURES

/*
 a symtable instance on the stack
*/
typedef struct symtable_instance_t {
    symtable_bvs_node_t *symtable;
    struct symtable_instance_t *next;
} symtable_instance_t;
/*
 stack of symtables
*/
typedef struct {
    symtable_instance_t *top;
} symtable_stack_t;



//SYMTABLE FUNCTIONS

/*
 initialization of the symbol table
*/
symtable_bvs_node_t *symtable_initialize();

/* 
 symtable_insert helper function - returns greater number of the two
*/
int bvs_max(int n1, int n2);
/* 
 symtable_insert helper function - returns height from the current pointer
*/
int bvs_height(symtable_bvs_node_t *ptr);
/* 
 symtable_insert helper function - performs AVL tree rotation to the left, uses bvs_max and bvs_height
*/
symtable_bvs_node_t *symtable_left_rotate(symtable_bvs_node_t *y);
/* 
 symtable_insert helper function - performs AVL tree rotation to the right, uses bvs_max and bvs_height
*/
symtable_bvs_node_t *symtable_right_rotate(symtable_bvs_node_t *y);

/*
 returns the position of the found symbol node, or NULL if not found
 implementation is recursive
*/
symtable_bvs_node_t *symtable_find_symbol(symtable_bvs_node_t *rootPtr, char *k);

/*
 symtable_insert helper function - allocates a new node, allocates key string and inserts received symbol information into the node 
*/
symtable_bvs_node_t *symtable_create_node(char *key, symbol_data_t symb);
/*
 inserts new node as a leaf node of a binary tree, performs height balancing through rotations if necessary, 
 uses error() if insertion is not possible
 implementation is recursive
*/
symtable_bvs_node_t *symtable_insert(symtable_bvs_node_t *rootPtr, char *key, symbol_data_t symb);

void symtable_destructor(symtable_bvs_node_t *root_ptr);


//STACK FUNTIONS

/*
 initialization of the symbol table stack
*/
void symtable_stack_initialize(symtable_stack_t *stack);
/*
 puts new symbol table instance on top of the stack 
*/
void symtable_stack_push(symtable_stack_t *stack);
/*
 deletes the symbol table instance from top of the stack
*/
void symtable_stack_pop(symtable_stack_t *stack);

/*
 function searches for a symbol in all instances on the stack, returns location
*/
symtable_bvs_node_t *symtable_stack_search(symtable_stack_t *stack, char *key);


//LINKED LIST FUNCTIONS

/*
 initialization of the parameter list
*/
param_type_list_t *param_type_list_initialize(void);
/*
 inserts and allocates new parameter's data type as the last element
*/
void param_type_list_insert(param_type_list_t *l, data_type type);
/*
 frees entire parameter's types list
*/
//void param_list_destroy(param_type_list_t *l);

#endif