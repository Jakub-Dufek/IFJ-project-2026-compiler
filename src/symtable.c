/**
* @file symtable.c
* @internal Part of IFJ-project-2026-compiler
*
* @brief symbol table structure
* @author Ondřej Kycl (xkyclon00) 
* @date 04.10.2026
* 
* @details This file contains symbol table binary tree along with the user functions
*/

// libraries
#include "symtable.h"
#include <stdlib.h>
#include <string.h>

TNode * symtable_init() {
    return NULL;
}

TNode *FindSymbol(TNode *rootPtr, char *k){
    if (rootPtr == NULL){
        return false;
    }
    else {
        if (strcmp(rootPtr->key, k) != 0){
            if (strcmp(rootPtr->key, k) > 0){
                return FindSymbol(rootPtr->lPtr, k);
            }
            else {
                return FindSymbol(rootPtr->rPtr, k);
            }
        }
        else {
            return rootPtr;
        }          
    } 
}

TNode *CreateNode(char *key, TSymb symb){
    TNode *newNode = (TNode *) malloc(sizeof(TNode));
    if (newNode == NULL){
        return NULL;
    }
    newNode->key = key;
    newNode->symb = symb;
    newNode->lPtr = NULL;
    newNode->lPtr = NULL;
}

TNode *Insert(TNode *rootPtr, char *key, TSymb symb){
    if (rootPtr == NULL){
        return CreateNode(key, symb);
    }
    else {
        if (strcmp(rootPtr->key, key) > 0){
            rootPtr->lPtr = Insert(rootPtr->lPtr, key, symb);
        }
        else{
            if (strcmp(rootPtr->key, key) < 0){
                rootPtr->rPtr = Insert(rootPtr->rPtr, key, symb);
            }
            else{
                rootPtr->symb = symb;
            }
        }
    }
}