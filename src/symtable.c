/**
* @file symtable.c
* @internal Part of IFJ-project-2026-compiler
*
* @brief symbol table with structure implementation
* @author Ondřej Kycl (xkyclon00) 
* @date 04.10.2026
* 
* @details This file contains a symbol table implemented with height self-balancing binary tree data structure
*/

//TODO : Upravit popisky,
//       .clang-format,
//       GetData funkce
//       stejné identifikátory 
//       volání funkcí před lexikální definicí?
//       line on which error ocurred in ifj26, stejné identifikátory (řádek v insert!)

#include "symtable.h"

symtable_bvs_node_t *symtable_initialize() {
    return NULL;
}

int bvs_max(int n1, int n2){ 
    if (n1 > n2) {
        return n1;
    }
    return n2;
}
int bvs_height(symtable_bvs_node_t *ptr){
    if (ptr == NULL) {
        return 0;
    }
    return ptr->height;
}

symtable_bvs_node_t *left_rotate(symtable_bvs_node_t *y){
    symtable_bvs_node_t *x = y->r_ptr;
    y->r_ptr = x->l_ptr;
    x->l_ptr = y;
    // switched nodes weight correction
    y->height = bvs_max(bvs_height(y->l_ptr), bvs_height(y->r_ptr)) + 1;
    x->height = bvs_max(bvs_height(x->l_ptr), bvs_height(x->r_ptr)) + 1;

    return x;
}
symtable_bvs_node_t *right_rotate(symtable_bvs_node_t *y){
    symtable_bvs_node_t *x = y->l_ptr;
    y->l_ptr = x->r_ptr;
    x->r_ptr = y;
    // switched nodes height correction
    y->height = bvs_max(bvs_height(y->l_ptr), bvs_height(y->r_ptr)) + 1;
    x->height = bvs_max(bvs_height(x->l_ptr), bvs_height(x->r_ptr)) + 1;

    return x;
}

symtable_bvs_node_t *symtable_find_symbol(symtable_bvs_node_t *root_ptr, char *k){
    if (root_ptr == NULL){
        return NULL;
    }
    else {
        if (strcmp(root_ptr->key, k) != 0){
            if (strcmp(root_ptr->key, k) > 0){
                return symtable_find_symbol(root_ptr->l_ptr, k);
            }
            else {
                return symtable_find_symbol(root_ptr->r_ptr, k);
            }
        }
        else {
            return root_ptr;
        }          
    } 
}

symtable_bvs_node_t *symtable_create_node(char *key, symbol_data_t symb){
    symtable_bvs_node_t *new_node = (symtable_bvs_node_t *) malloc(sizeof(symtable_bvs_node_t));
    if (new_node == NULL){
        return NULL;
    }
    new_node->key = (char *)malloc((strlen(key) + 1) * sizeof(char));
    strcpy(new_node->key, key);

    new_node->symbol_data = symb;
    new_node->l_ptr = NULL;
    new_node->r_ptr = NULL;
    new_node->height = 1;

    return new_node;
}

symtable_bvs_node_t *symtable_insert(symtable_bvs_node_t *root_ptr, char *key, symbol_data_t symb){
    if (root_ptr == NULL){
        return symtable_create_node(key, symb);
    }
    else {
        if (strcmp(root_ptr->key, key) > 0){
            root_ptr->l_ptr = symtable_insert(root_ptr->l_ptr, key, symb);
        }
        else{
            if (strcmp(root_ptr->key, key) < 0){
                root_ptr->r_ptr = symtable_insert(root_ptr->r_ptr, key, symb);
            }
            else{
                root_ptr->symbol_data = symb;
                error_exit(SEM_REDEF_ERR, 0, "Redefinition of variable"); // <-- return to this
                //return NULL ?;
            }
        }

        root_ptr->height = bvs_max(bvs_height(root_ptr->l_ptr), bvs_height(root_ptr->r_ptr)) + 1;
        root_ptr->weight = bvs_height(root_ptr->r_ptr) - bvs_height(root_ptr->l_ptr);
        
        if (root_ptr->weight == -2){
            if (strcmp(root_ptr->l_ptr->key, key) > 0){
                root_ptr = right_rotate(root_ptr);
            }
            else{
                root_ptr->l_ptr = left_rotate(root_ptr->l_ptr);
                root_ptr = right_rotate(root_ptr);
            }
        }
        else if (root_ptr->weight == 2){
            if (strcmp(root_ptr->r_ptr->key, key) < 0){
                root_ptr = left_rotate(root_ptr);
            }
            else{
                root_ptr->r_ptr = right_rotate(root_ptr->r_ptr);
                root_ptr = left_rotate(root_ptr);
            }
        }

        return root_ptr;
    }
}

void symtable_destructor(symtable_bvs_node_t *root_ptr){
    if(root_ptr == NULL){
        return;
    }
    if (root_ptr->l_ptr != NULL){
        symtable_destructor(root_ptr->l_ptr);
    }
    if (root_ptr->r_ptr != NULL){
        symtable_destructor(root_ptr->r_ptr);
    }
    free(root_ptr->key);
    free(root_ptr);
}

