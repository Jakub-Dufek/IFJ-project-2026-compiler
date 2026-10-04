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

typedef struct tsymb {
    DataType datatype;
    IdType idtype;
    unsigned index; //val, var
    bool defined; // func

    ///func PARAMETERS and its types !!!

    //// label ???
} TSymb;

typedef struct tnode {
    char *key; // symbol/identifier used for binary search
    TSymb symb;
    struct tnode *lPtr;
    struct tnode *rPtr;
    // int weight;
} TNode;   

#ifndef SYMTABLE
#define SYMTABLE

TNode * symtable_init();

TNode *FindSymbol(TNode *rootPtr, char *k);

TNode *CreateNode(char *key, TSymb symb); // Helper function
TNode *Insert(TNode *rootPtr, char *key, TSymb symb);
void table_dst();

#endif