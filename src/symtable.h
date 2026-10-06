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

typedef enum { 
    TYPE_UNDEF = 0,
    TYPE_UNIT,
    TYPE_INT,
    TYPE_DOUBLE,
    TYPE_STRING,
    TYPE_BOOLEAN
} DataType;

typedef enum {
    TYPE_UNDEF = 0, 
    TYPE_VAL,
    TYPE_VAR,
    TYPE_FUNC,
    TYPE_LABEL // <-- Come back to this
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
    //int weight;
} bvs_node_t;   



bvs_node_t * symtable_init();

bvs_node_t *FindSymbol(bvs_node_t *rootPtr, char *k);

bvs_node_t *CreateNode(char *key, symbol_data_t symb); // insert helper function
bvs_node_t *Insert(bvs_node_t *rootPtr, char *key, symbol_data_t symb);

bvs_node_t *BVSMin(bvs_node_t *root_ptr); // bvs_delete helper function
bvs_node_t *BVSDelete(bvs_node_t *root_ptr, char *k);
bvs_node_t *bvs_delete_match(bvs_node_t *root_ptr); // bvs_delete helper function
//void table_dst();

#endif