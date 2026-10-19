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
#include "../src/error.h"
#include "stdbool.h"
#include <stdlib.h>
#include <string.h> 

//definitions:
/* 
 description of data types with enum from "structures.h":
 0-6 - (T_UNDEFINED = NO_VALUE = 0, T_KEY_INT = 1, T_KEY_DOUBLE = 2,
 T_KEY_STRING = 3, T_KEY_BOOLEAN = 4, T_KEY_UNIT = 5, T_NO_TYPE = 6), 
 token_type is renamed to data_type to match situation better 
*/
typedef token_type data_type; // 

/* 
 id_type description of an id type of a symbol with enum
*/
typedef enum {
    ID_UNDEF = 0, 
    ID_VAL,
    ID_VAR,
    ID_FUNC,
    ID_LABEL // <-- Come back to this
} id_type;


/* 
 symbol_data_t is a structure for storage of data related to a symbol
*/
typedef struct symbol_data_t {
    data_type data_type; 
    id_type id_type;
    unsigned index; //var, val <-- Come back to this
    bool defined; //func <-- Come back to this

    /// int value;? <-- Come back to this
    ///func PARAMETERS and its types !!! <-- Come back to this
    /// is @main ??? <-- Come back to this
    //// label ??? <-- Come back to this
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

#endif