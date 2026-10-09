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

//TODO : Upravit popisky,
//       GetData funkce
//       stejné identifikátory
//       volání funkcí před lexikální definicí?
//       Spojit enumy s Kubou?
//       popsat funkce, prejmenovat ?
//       MAX_LENGTH pro Key/symbol?

// libraries
#include "symtable.h"
#include <stdlib.h>
#include <string.h> 

bvs_node_t *symtable_initialize() {
    return NULL;
}

bvs_node_t *find_symbol(bvs_node_t *root_ptr, char *k){
    if (root_ptr == NULL){
        return NULL;
    }
    else {
        if (strcmp(root_ptr->key, k) != 0){
            if (strcmp(root_ptr->key, k) > 0){
                return find_symbol(root_ptr->l_ptr, k);
            }
            else {
                return find_symbol(root_ptr->r_ptr, k);
            }
        }
        else {
            return root_ptr;
        }          
    } 
}

bvs_node_t *create_node(char *key, symbol_data_t symb){
    bvs_node_t *new_node = (bvs_node_t *) malloc(sizeof(bvs_node_t));
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

bvs_node_t *insert(bvs_node_t *root_ptr, char *key, symbol_data_t symb){
    if (root_ptr == NULL){
        return create_node(key, symb);
    }
    else {
        if (strcmp(root_ptr->key, key) > 0){
            root_ptr->l_ptr = insert(root_ptr->l_ptr, key, symb);
        }
        else{
            if (strcmp(root_ptr->key, key) < 0){
                root_ptr->r_ptr = insert(root_ptr->r_ptr, key, symb);
            }
            else{
                root_ptr->symbol_data = symb;
                //return NULL ?;
                //error
            }
        }

        root_ptr->height = max(height(root_ptr->l_ptr), height(root_ptr->r_ptr)) + 1;
        root_ptr->weight = height(root_ptr->r_ptr) - height(root_ptr->l_ptr);
        
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

int max(int n1, int n2){ 
    if (n1 > n2) {
        return n1;
    }
    return n2;
}
int height(bvs_node_t *ptr){
    if (ptr == NULL) {
        return 0;
    }
    return ptr->height;
}

bvs_node_t *left_rotate(bvs_node_t *y){
    bvs_node_t *x = y->r_ptr;
    y->r_ptr = x->l_ptr;
    x->l_ptr = y;
    // switched nodes weight correction
    y->height = max(height(y->l_ptr), height(y->r_ptr)) + 1;
    x->height = max(height(x->l_ptr), height(x->r_ptr)) + 1;

    return x;
}
bvs_node_t *right_rotate(bvs_node_t *y){
    bvs_node_t *x = y->l_ptr;
    y->l_ptr = x->r_ptr;
    x->r_ptr = y;
    // switched nodes height correction
    y->height = max(height(y->l_ptr), height(y->r_ptr)) + 1;
    x->height = max(height(x->l_ptr), height(x->r_ptr)) + 1;

    return x;
}

void symtable_destructor(bvs_node_t *root_ptr){
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

