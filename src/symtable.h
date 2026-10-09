/**
* @file symtable.h
* @internal Part of IFJ-project-2026-compiler
*
* @brief symbol table structure
* @author Ondřej Kycl (xkyclon00) 
* @date 04.10.2026
* 
* @details This file contains definitions of functions used in symtable.c
*/
#ifndef SYMTABLE
#define SYMTABLE

//libraries
#include "structures.h" ///////// necessary?????
#include "stdbool.h"

typedef enum { 
    TYPE_UNDEF = 0,
    TYPE_UNIT,
    TYPE_INT,
    TYPE_DOUBLE,
    TYPE_STRING,
    TYPE_BOOLEAN
} DataType;

typedef enum {
    ID_UNDEF = 0, 
    ID_VAL,
    ID_VAR,
    ID_FUNC,
    ID_LABEL // <-- Come back to this
} IdType;

typedef struct symbol_data_t {
    DataType data_type;
    IdType id_type;
    unsigned index; //val, var
    bool defined; // func

    ///func PARAMETERS and its types !!!
    /// is @main ???
    //// label ???
} symbol_data_t;

typedef struct bvs_node_t {
    char *key; // symbol/identifier used for binary search
    symbol_data_t symbol_data;
    struct bvs_node_t *l_ptr;
    struct bvs_node_t *r_ptr;
    int weight;
    unsigned height;
} bvs_node_t;   



bvs_node_t *symtable_initialize();

bvs_node_t *find_symbol(bvs_node_t *rootPtr, char *k);

bvs_node_t *create_node(char *key, symbol_data_t symb); // insert helper function
bvs_node_t *insert(bvs_node_t *rootPtr, char *key, symbol_data_t symb);

int max(int n1, int n2);
int height(bvs_node_t *ptr);
bvs_node_t *left_rotate(bvs_node_t *y);
bvs_node_t *right_rotate(bvs_node_t *y);

void symtable_destructor(bvs_node_t *root_ptr);

#endif